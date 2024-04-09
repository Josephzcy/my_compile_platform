/******************************************************
    Author      : wangnb
    Date        : 2022/4/11
    Description :
        1.
    -----------------------------
    Change      :
        1. create ExtrinsCalib.cpp
*******************************************************/
#include "aipilot/services/dfg_avm_calib/src/extrins_calib.h"

#include <fstream>

#include "aipilot/services/dfg_avm_calib/src/common_layout.h"
#include "aipilot/services/dfg_avm_calib/src/jx65_layout.h"
#include "aipilot/services/dfg_avm_mapping/src/avm_parse.h"
#include "glog/logging.h"
#include "json.hpp"

#define AVM_PI (3.141592654)
#define RAD2DEGREE(x) (x * 180 / AVM_PI)
#define INPUT_CAMERA_NUM (4u)

#define BIRD_VIEW_WIDTH (960)
#define BIRD_VIEW_HEIGHT (1440)
#define SIGHT_WIDTH (3.0f)

namespace minieye {
namespace avm {
AvmErrorCode AvmExtrinsCalib::init(const std::vector<std::string> &intrins_file,
                                   const std::string &layout_file,
                                   const std::string &car_info_file) {
  AvmErrorCode ret_code;
  ret_code = initAvmLayout(layout_file, car_info_file);
  if (AVM_NORMAL_STATE == ret_code) {
    int camera_num = intrins_file.size();
    camera_params_.resize(camera_num);
    Ks_.resize(camera_num);
    for (int inum = 0; inum < camera_num; ++inum) {
      ret_code = initCameraModel(intrins_file[inum]);
      if (AVM_NORMAL_STATE != ret_code) {
        break;
      }
    }
  }
  if (AVM_NORMAL_STATE == ret_code && intrins_file.size() != INPUT_CAMERA_NUM) {
    ret_code = AVM_ERROR_ABNORMAL_INTRINS;
  }
  if (AVM_NORMAL_STATE == ret_code) {
    bool flag = avm_mapper_.InitCameraParams(intrins_file);
    if (!flag) {
      ret_code = AVM_ERROR_ABNORMAL_INTRINS;
    }
  }
  return ret_code;
}

AvmErrorCode AvmExtrinsCalib::initCameraModel(const std::string &intrins_str) {
  AvmErrorCode ret_code = AVM_NORMAL_STATE;
  CameraParam camera_param;
  float balance = 1.0f;
  bool flag = parseIntrinsJson(intrins_str, &camera_param);
  if (flag) {
    uint32_t camera_id = camera_param.camera_id();
    uint32_t ch = (uint32_t)std::log2(camera_id);
    img_size_ =
        cv::Size(camera_param.image_width(), camera_param.image_height());
    camera_params_[ch] = camera_param;

    float fu = camera_param.focal_u();
    float fv = camera_param.focal_v();
    float cu = camera_param.cu();
    float cv = camera_param.cv();
    float s = 0;
    if (MEI == camera_param.prj_model()) {
      s = camera_param.distort_coeffs(0);
    }
    cv::Mat K =
        (cv::Mat_<double>(3, 3) << fu, s, cu, 0.0, fv, cv, 0.0, 0.0, 1.0);
    int chn = static_cast<int>(std::log2(camera_param.camera_id()));
    Ks_[chn] = K.clone();
  } else {
    ret_code = AVM_ERROR_OPENFILE;
  }

  return ret_code;
}

AvmErrorCode AvmExtrinsCalib::initAvmLayout(const std::string &extrins_str,
                                            const std::string &car_info_str) {
  AvmErrorCode ret_code = AVM_NORMAL_STATE;

  int layout_type = 0;

  nlohmann::json config_json;
  std::ifstream fin(extrins_str);
  if (!fin.is_open()) {
    LOG(ERROR) << "failed to load extrins_file file : " << extrins_str;
    ret_code = AVM_ERROR_OPENFILE;
  } else {
    fin >> config_json;
    layout_type = config_json["layout"];
  }
  fin.close();
  if (AVM_NORMAL_STATE == ret_code) {
    switch (layout_type) {
    case 0:
      avm_layout_ = std::make_shared<CommonLayout>();
      break;
    case 1:
      avm_layout_ = std::make_shared<JX65Layout>();
      break;
    case 2:
      avm_layout_ = std::make_shared<JX65Layout>();
      break;
    default:
      LOG(ERROR) << "unsupport avm calibrated layout";
      ret_code = AVM_ERROR_ABNORMAL_EXTRINS;
    }
  }
  if (AVM_NORMAL_STATE == ret_code) {
    ret_code = (AvmErrorCode)avm_layout_->initLayout(extrins_str, car_info_str);
  }
  // jx65项目有额外信息可供使用
  if (AVM_NORMAL_STATE == ret_code && 1 == layout_type) {
    auto layout_ptr = (JX65Layout *)avm_layout_.get();
    layout_ptr->getExtraCheckerBoardInfo(extra_CBSize_, extra_CBPoints_);
  }
  return ret_code;
}

int32_t AvmExtrinsCalib::writeJson(uint32_t chn,
                                   const std::string &extrins_json,
                                   const cv::Mat &H, const cv::Mat &rvec,
                                   const cv::Mat &tvec) {
  LOG(INFO) << "success to calib extrins, write";

  nlohmann::json root;
  root["camera_id"] = 1u << chn;
  root["install_direction"] = 1u << chn;
  root["prj_model"] = camera_params_[chn].prj_model();
  root["image_width"] = img_size_.width;
  root["image_height"] = img_size_.height;
  root["focal_u"] = camera_params_[chn].focal_u();
  root["focal_v"] = camera_params_[chn].focal_v();
  root["cu"] = camera_params_[chn].cu();
  root["cv"] = camera_params_[chn].cv();

  uint32_t start_inum = 0;
  if (2 == camera_params_[chn].prj_model()) {
    root["s"] = camera_params_[chn].distort_coeffs(0);
    start_inum = 1;
  }
  for (uint32_t inum = start_inum;
       inum < (uint32_t)camera_params_[chn].distort_coeffs_size(); ++inum) {
    root["distort_coeffs"].push_back(camera_params_[chn].distort_coeffs(inum));
  }

  // homo
  const auto h0 = H.ptr<float>(0);
  const auto h1 = H.ptr<float>(1);
  const auto h2 = H.ptr<float>(2);
  root["vcsgnd2img"] = {h0[0], h0[1], h0[2], h1[0], h1[1],
                        h1[2], h2[0], h2[1], h2[2]};
  root["pitch"] = rvec.at<float>(0, 0);
  root["yaw"] = rvec.at<float>(0, 1);
  root["roll"] = rvec.at<float>(0, 2);

  root["pos"] = {tvec.at<float>(0, 0), tvec.at<float>(0, 1),
                 tvec.at<float>(0, 2)};

  for (int inum = 0; inum < 3; ++inum) {
    root["rvec"].push_back(rvec.at<float>(0, inum));
    root["tvec"].push_back(tvec.at<float>(0, inum));
  }

  // 输出到文件
  std::ofstream os;
  os.open(extrins_json, std::ios::out);
  os << root.dump(4) << std::endl;
  os.close();
  return 0;
}

AvmErrorCode
AvmExtrinsCalib::detectConner(const cv::Mat &img, const cv::Size &boardSize,
                              std::vector<cv::Point2d> &conner_pts) {
  AvmErrorCode ret_code = AVM_NORMAL_STATE;
  conner_pts.clear();
  std::vector<cv::Point2f> checker_board_points;
  if (img.empty()) {
    LOG(INFO) << "failed to read image";
    ret_code = AVM_ERROR_ENPTY_IMAGE;
  } else {
    int flags = cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_ADAPTIVE_THRESH;
    bool found =
        cv::findChessboardCorners(img, boardSize, checker_board_points, flags);
    if (found) {
      cv::Mat viewGray;
      cv::cvtColor(img, viewGray, cv::COLOR_BGR2GRAY);
      cv::TermCriteria criteria_subpix(
          cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1);
      cv::cornerSubPix(viewGray, checker_board_points, cv::Size(11, 11),
                       cv::Size(-1, -1), criteria_subpix);
      for (uint32_t inum = 0; inum < checker_board_points.size(); ++inum) {
        conner_pts.emplace_back(cv::Point2d(checker_board_points[inum].x,
                                            checker_board_points[inum].y));
      }
    } else {
      LOG(INFO) << "failed to detect conner";
      ret_code = AVM_ERROR_CONNER_NOT_DETECTED;
    }
  }
#if 0
  cv::Mat img_test = img.clone();
  cv::drawChessboardCorners(img_test, boardSize, checker_board_points, 1);
  cv::imshow("detect_conner", img_test);
  cv::waitKey(0);
#endif

  return ret_code;
}

void AvmExtrinsCalib::calcExtrins(uint32_t chn,
                                  const std::vector<cv::Point3d> &world_pts,
                                  const std::vector<cv::Point2d> &pix_pts,
                                  cv::Mat &R, cv::Mat &T, cv::Mat &H,
                                  bool useExtrinsGusse) {
  std::vector<cv::Point2d> undistortPts;
  uint32_t camera_direction = 1 << chn;
  std::vector<cv::Point2f> float_undist;
  for (uint32_t inum = 0; inum < pix_pts.size(); ++inum) {
    cv::Point2f undist_pt;
    avm_mapper_.UnDistort(camera_direction,
                          cv::Point2f(pix_pts[inum].x, pix_pts[inum].y),
                          undist_pt);
    undistortPts.emplace_back(undist_pt.x, undist_pt.y);
  }
  cv::solvePnP(world_pts, undistortPts, Ks_[chn], cv::Mat(), R, T,
               useExtrinsGusse, cv::SOLVEPNP_ITERATIVE);
  // 求世界坐标到像素坐标的单应性矩阵
  H = cv::findHomography(world_pts, undistortPts, cv::RANSAC, 0.2);

#ifdef LINUX_PC
  std::ofstream fout(std::to_string(chn) + "_pts.txt");
  for (size_t inum = 0; inum < pix_pts.size(); ++inum) {
    fout << pix_pts[inum].x << ", " << pix_pts[inum].y << std::endl;
    fout << world_pts[inum].x << ", " << world_pts[inum].y << ", "
         << world_pts[inum].z << std::endl;
  }
  fout.close();
#endif

  R.convertTo(R, CV_32F);
  T.convertTo(T, CV_32F);
  H.convertTo(H, CV_32F);
}

void AvmExtrinsCalib::undistImg(uint32_t chn, const cv::Mat &src_img,
                                cv::Mat &undist_img) {
  undist_img.create(src_img.size(), CV_8UC3);
  undist_img.setTo(0);

  cv::Mat mapx, mapy;
  mapx.create(src_img.size(), CV_32F);
  mapy.create(src_img.size(), CV_32F);
  int width = src_img.cols;
  float *data_mapx = (float *)mapx.data;
  float *data_mapy = (float *)mapy.data;

  uint32_t camera_direction = 1 << chn;

  for (int irow = 0; irow < src_img.rows; ++irow) {
    for (int icol = 0; icol < src_img.cols; ++icol) {
      cv::Point2f distort_pt;
      avm_mapper_.Distort(camera_direction, cv::Point2f(icol, irow),
                          distort_pt);

      *(data_mapx + irow * width + icol) = (float)distort_pt.x;
      *(data_mapy + irow * width + icol) = (float)distort_pt.y;
    }
  }

  cv::remap(src_img, undist_img, mapx, mapy, cv::INTER_LINEAR);
}

AvmErrorCode AvmExtrinsCalib::calib(CamDirect camera_id,
                                    const unsigned char *img_data,
                                    const std::string &calib_json,
                                    float &remap_residual) {
  AvmErrorCode ret_code;
  uint32_t chn = std::log2((int)camera_id);
  std::vector<cv::Point3d> world_pts;
  std::vector<cv::Point2d> conner_pixe_pts;
  ret_code = static_cast<AvmErrorCode>(
      avm_layout_->getWorldPt((uint32_t)camera_id, world_pts));

  cv::Mat img;
  img = cv::Mat(img_size_, CV_8UC3);
  if (nullptr != img_data) {
    memcpy(img.data, img_data,
           sizeof(unsigned char) * img_size_.width * img_size_.height * 3);
  } else {
    ret_code = AVM_ERROR_ENPTY_IMAGE;
  }

  if (AVM_NORMAL_STATE == ret_code) {
    cv::Size boardSize;
    avm_layout_->getCheckerBoardConnerSize(boardSize);
    ret_code = detectConner(img, boardSize, conner_pixe_pts);
    cv::rectangle(img, conner_pixe_pts[0],
                  conner_pixe_pts[conner_pixe_pts.size() - 1],
                  cv::Scalar(0, 0, 0), -1);
    cv::Mat rvec, tvec, H;
    if (ret_code == AVM_NORMAL_STATE) {
      calcExtrins(chn, world_pts, conner_pixe_pts, rvec, tvec, H);
      if (1 == avm_layout_->getLayoutType()) {
        ret_code = static_cast<AvmErrorCode>(
            refineExtrins(chn, img, rvec, tvec, H, world_pts, conner_pixe_pts));
      }
    }
    if (ret_code == AVM_NORMAL_STATE) {
      writeJson(chn, calib_json, H, rvec, tvec);
      remap_residual = getResidual(chn, world_pts, conner_pixe_pts, rvec, tvec);
    }
  }
  return ret_code;
}

float AvmExtrinsCalib::getResidual(uint32_t chn,
                                   const std::vector<cv::Point3d> &world_pts,
                                   const std::vector<cv::Point2d> &pix_pts,
                                   const cv::Mat &rvec, const cv::Mat &tvec) {
  float rsme = 0;
  for (size_t inum = 0; inum < pix_pts.size(); ++inum) {
    cv::Point3d reproj_3d_pt;
    unproject(chn, rvec, tvec, pix_pts[inum], reproj_3d_pt);
    double sum_err = std::pow(reproj_3d_pt.x - world_pts[inum].x, 2) +
                     std::pow(reproj_3d_pt.y - world_pts[inum].y, 2) +
                     std::pow(reproj_3d_pt.z - world_pts[inum].z, 2);
    //    LOG(ERROR) << "chn: " << chn << ", ipt_num: " << inum
    //               << ", err: " << std::sqrt(sum_err);
    rsme += std::sqrt(sum_err);
  }
  rsme = rsme / pix_pts.size();
  return rsme;
}

int AvmExtrinsCalib::unproject(int chn, const cv::Mat &rvec,
                               const cv::Mat &tvec, const cv::Point2d &pix,
                               cv::Point3d &pw) {
  int ret_code = 0;
  uint32_t camera_direction = 1 << chn;
  cv::Vec3d om = rvec.depth() == CV_32F ? (cv::Vec3d)*rvec.ptr<cv::Vec3f>()
                                        : *rvec.ptr<cv::Vec3d>();
  cv::Vec3d T = tvec.depth() == CV_32F ? (cv::Vec3d)*tvec.ptr<cv::Vec3f>()
                                       : *tvec.ptr<cv::Vec3d>();
  cv::Matx33d R;
  cv::Matx<double, 3, 9> dRdom;
  Rodrigues(om, R, dRdom);
  cv::Point2f undist_pt;
  avm_mapper_.UnDistort(camera_direction, cv::Point2f(pix.x, pix.y), undist_pt);
  cv::Vec3d proj_vec = cv::Vec3d(undist_pt.x, undist_pt.y, 1.0f);
  proj_vec = ((cv::Matx33d)(Ks_[chn])).inv() * proj_vec;
  proj_vec /= proj_vec[2];
  R.val[2] = T[0];
  R.val[5] = T[1];
  R.val[8] = T[2];

  cv::Vec3d proj_pw = R.inv() * proj_vec;
  proj_pw /= proj_pw[2];

  pw.x = proj_pw[0];
  pw.y = proj_pw[1];
  pw.z = 0;
  return ret_code;
}

int AvmExtrinsCalib::project(int chn, const cv::Mat &rvec, const cv::Mat &tvec,
                             const cv::Point3d &pw, cv::Point2d &proj_pt) {
  int ret_code = 0;
  cv::Vec3d om = rvec.depth() == CV_32F ? (cv::Vec3d)*rvec.ptr<cv::Vec3f>()
                                        : *rvec.ptr<cv::Vec3d>();
  cv::Vec3d T = tvec.depth() == CV_32F ? (cv::Vec3d)*tvec.ptr<cv::Vec3f>()
                                       : *tvec.ptr<cv::Vec3d>();
  cv::Matx33d R;
  cv::Matx<double, 3, 9> dRdom;
  Rodrigues(om, R, dRdom);

  cv::Vec3d Xw(pw.x, pw.y, pw.z);
  cv::Vec3d Xc = (cv::Vec3d)(R * Xw + T);
  if (Xc[2] > -DBL_EPSILON && Xc[2] < DBL_EPSILON) {
    Xc[2] += DBL_EPSILON;
  }
  Xc = (cv::Matx33d)Ks_[chn] * Xc;
  Xc /= Xc[2];
  uint32_t camera_direction = 1 << chn;
  cv::Point2f f32_pt;
  avm_mapper_.Distort(camera_direction, cv::Point2f(Xc[0], Xc[1]), f32_pt);
  proj_pt = cv::Point2d(f32_pt.x, f32_pt.y);
  return ret_code;
}

/**
 * 根据标定初始结果生成鸟瞰图，以便棋盘格角点检测
 * 图像与外参输入顺序：前后左右
 * @param rvecs
 * @param tvecs
 * @param imgs
 * @param bev_imgs
 * @param assistant_rois
 */
void AvmExtrinsCalib::remapping(int chn, const cv::Mat &rvec,
                                const cv::Mat &tvec, const cv::Mat &img,
                                cv::Mat &bev_img, cv::Rect assistant_rois[2]) {
  const cv::Size outImgSize(BIRD_VIEW_WIDTH, BIRD_VIEW_HEIGHT);
  CarInfo car_info = avm_layout_->getCarInfo();

  float sight_width = SIGHT_WIDTH + car_info.car_width() / 2.0f;
  float sight_height = (sight_width / BIRD_VIEW_WIDTH * BIRD_VIEW_HEIGHT);

  float t_offset = -car_info.car_length() / 2 + car_info.car_front_overhang() +
                   car_info.car_wheelbase();

  float x_start = (float)(sight_height);
  float y_start = (float)(sight_width);
  float x_stride = (float)sight_height / (float)(BIRD_VIEW_HEIGHT / 2);
  float y_stride = (float)sight_width / (float)(BIRD_VIEW_WIDTH / 2);

  float car_length_pix = car_info.car_length() / x_stride;
  float car_width_pix = car_info.car_width() / y_stride;
  std::vector<cv::Rect> rois = {
      cv::Rect(0, 0, outImgSize.width,
               (outImgSize.height - car_length_pix) / 2),
      cv::Rect(0, (outImgSize.height + car_length_pix) / 2, outImgSize.width,
               (outImgSize.height - car_length_pix) / 2),
      cv::Rect(0, 0, (outImgSize.width - car_width_pix) / 2, outImgSize.height),
      cv::Rect((outImgSize.width + car_width_pix) / 2, 0,
               (outImgSize.width - car_width_pix) / 2, outImgSize.height)};

  int chess_width = (outImgSize.width - car_width_pix) / 2;
  int chess_height = (outImgSize.height - car_length_pix) / 2;

  // front-left, left_rear, rear_right, right-front
  std::vector<cv::Rect> chess_rois = {
      cv::Rect(0, 0, chess_width, chess_height),
      cv::Rect(0, (outImgSize.height + car_length_pix) / 2, chess_width,
               chess_height),
      cv::Rect(car_width_pix + chess_width, car_length_pix + chess_height,
               chess_width, chess_height),
      cv::Rect((outImgSize.width + car_width_pix) / 2, 0, chess_width,
               chess_height)};

  // 前后左右小棋盘格在逆时针顺序存储下的索引
  std::vector<int> indexs = {0, 3, 1, 2, 0, 1, 3, 2};

  assistant_rois[0] = chess_rois[indexs[chn * 2 + 0]];
  assistant_rois[1] = chess_rois[indexs[chn * 2 + 1]];

  int32_t camera_id = 1 << chn;
  bev_img.create(outImgSize, CV_8UC3);
  bev_img.setTo(cv::Scalar(0, 0, 0));

  cv::Mat mapx(outImgSize, CV_32F);
  cv::Mat mapy(outImgSize, CV_32F);
  mapx.setTo(0);
  mapy.setTo(0);
  for (int32_t irow = rois[chn].y; irow < rois[chn].y + rois[chn].height;
       ++irow) {
    float *mapx_ptr = mapx.ptr<float>(irow);
    float *mapy_ptr = mapy.ptr<float>(irow);
    for (int32_t icol = rois[chn].x; icol < rois[chn].x + rois[chn].width;
         ++icol) {
      cv::Point3d pw(x_start - irow * x_stride, y_start - icol * y_stride, 0.0);
      pw.x += t_offset;
      cv::Point2d proj2d;
      project(chn, rvec, tvec, pw, proj2d);

      mapx_ptr[icol] = proj2d.x;
      mapy_ptr[icol] = proj2d.y;
    }
  }

  cv::remap(img, bev_img, mapx, mapy, cv::INTER_CUBIC);
}

void AvmExtrinsCalib::getAllConner(int chn, const cv::Mat &bev_imgs,
                                   const cv::Rect assistant_rois[2],
                                   std::vector<cv::Point2d> &allConners) {
  allConners.clear();

  for (int i = 0; i < 2; ++i) {
    std::vector<cv::Point2d> extra_chess_board;
    cv::Rect roi = assistant_rois[i];
    cv::Mat chess_img = bev_imgs(roi);
    AvmErrorCode ret_code =
        detectConner(chess_img, extra_CBSize_, extra_chess_board);
    if (AVM_NORMAL_STATE != ret_code) {
      break;
    }
    reorderConner(extra_chess_board);
    transPoint(chn, i, roi.size(), extra_chess_board);

#if 0
            cv::Mat img_test = bev_imgs;
            std::vector<cv::Point2f> test_cb;
            for (size_t inum = 0; inum < 9; ++inum) {
              test_cb.push_back(
                  cv::Point2f(extra_chess_board[inum].x, extra_chess_board[inum].y));
            }
            cv::drawChessboardCorners(img_test, cv::Size(3, 3), test_cb, 1);
            cv::imshow("point_reorder", img_test);
            cv::waitKey(0);
#endif

    allConners.insert(allConners.end(), extra_chess_board.begin(),
                      extra_chess_board.end());
  }
}

void AvmExtrinsCalib::transPoint(int chn, int sub_cb,
                                 const cv::Size &assistan_size,
                                 std::vector<cv::Point2d> &all_conners) {
  cv::Size img_size(BIRD_VIEW_WIDTH, BIRD_VIEW_HEIGHT);
  // 逆时针确定棋盘格角点区域起点
  std::vector<cv::Point2d> start_pts = {
      cv::Point2d(0, 0), cv::Point2d(0, img_size.height - assistan_size.height),
      cv::Point2d(img_size.width - assistan_size.width,
                  img_size.height - assistan_size.height),
      cv::Point2d(img_size.width - assistan_size.width, 0)};
  // 总共有八组角点数据，根据其区域索引进行角点位置迁移
  int indexs[] = {0, 3, 1, 2, 0, 1, 3, 2};
  for (size_t ipt_num = 0; ipt_num < all_conners.size(); ++ipt_num) {
    all_conners[ipt_num] += start_pts[indexs[chn * 2 + sub_cb]];
  }
}

int AvmExtrinsCalib::getOrderMode(const std::vector<cv::Point2d> &conners) {
  int mode = 0;
  /*****--1--*********--2--*******--3--******--4--**
   *     0  3  6  |   2 5 8   |   8 5 2   |   6 3 0
   *   1  4  7    |  1 4 7    |  7 4 1    |  7 4 1
   * 2  5  8      | 0 3 6     | 6 3 0     | 8 5 2
   ******--5--*********--6--*******--7--******--8--**
   *     0  1  2  |   6 7 8   |   8 7 6   |   2 1 0
   *   3  4  5    |  3 4 5    |  5 4 3    |  5 4 3
   * 6  7  8      | 0 1 2     | 2 1 0     | 8 7 6
   */
  // 排列模式1,4,5,8
  if (conners[0].y < conners[8].y) {
    if (conners[2].y > conners[6].y) {   // 排列1，4
      if (conners[1].x < conners[7].x) { // 排列1
        mode = 1;
      } else { // 排列4
        mode = 4;
      }
    } else {                             // 排列5，8
      if (conners[3].x < conners[5].x) { // 排列5
        mode = 5;
      } else { // 排列8
        mode = 8;
      }
    }
  } else {                               // 排列模式，2，3，6，7
    if (conners[2].y < conners[6].y) {   // 排列2，3
      if (conners[1].x < conners[7].x) { // 排列2
        mode = 2;
      } else { // 排列3
        mode = 3;
      }
    } else {                             // 排列6，7
      if (conners[3].x < conners[5].x) { // 排列6
        mode = 6;
      } else { // 排列7
        mode = 7;
      }
    }
  }
  return mode;
}

// jx65共视区标靶特殊处理
void AvmExtrinsCalib::reorderConner(std::vector<cv::Point2d> &conners) {
  int mode = getOrderMode(conners);
  /*****--1--*********--2--*******--3--******--4--**
   *     0  3  6  |   2 5 8   |   8 5 2   |   6 3 0
   *   1  4  7    |  1 4 7    |  7 4 1    |  7 4 1
   * 2  5  8      | 0 3 6     | 6 3 0     | 8 5 2
   ******--5--*********--6--*******--7--******--8--**
   *     0  1  2  |   6 7 8   |   8 7 6   |   2 1 0
   *   3  4  5    |  3 4 5    |  5 4 3    |  5 4 3
   * 6  7  8      | 0 1 2     | 2 1 0     | 8 7 6
   */
  if (4 == mode || 2 == mode || 3 == mode || 1 == mode) {
    if (4 == mode) {
      // 排列4，转换至排列1
      std::swap(conners[0], conners[6]);
      std::swap(conners[1], conners[7]);
      std::swap(conners[2], conners[8]);
    } else if (2 == mode) {
      // 排列2，转换至排列1
      std::swap(conners[0], conners[2]);
      std::swap(conners[3], conners[5]);
      std::swap(conners[6], conners[8]);
    } else if (3 == mode) {
      // 排列3，转换至排列1
      std::swap(conners[0], conners[8]);
      std::swap(conners[1], conners[7]);
      std::swap(conners[2], conners[6]);
      std::swap(conners[3], conners[5]);
    }
    // 排列1转转至排列5
    std::swap(conners[1], conners[3]);
    std::swap(conners[2], conners[6]);
    std::swap(conners[5], conners[7]);
  } else {
    if (6 == mode) {
      // 排列6
      std::swap(conners[0], conners[6]);
      std::swap(conners[1], conners[7]);
      std::swap(conners[2], conners[8]);
    } else if (7 == mode) {
      // 排列7，转换至排列6
      std::swap(conners[0], conners[2]);
      std::swap(conners[3], conners[5]);
      std::swap(conners[6], conners[8]);

      // 转换至排列5
      std::swap(conners[0], conners[6]);
      std::swap(conners[1], conners[7]);
      std::swap(conners[2], conners[8]);
    } else if (8 == mode) {
      // 排列8，转换至排列5
      std::swap(conners[0], conners[2]);
      std::swap(conners[3], conners[5]);
      std::swap(conners[6], conners[8]);
    }
  }
}

void AvmExtrinsCalib::getFrontRearExtraConner(
    int chn, const cv::Mat &img, std::vector<cv::Point2d> &allConners) {
  allConners.clear();
  cv::Rect left_roi(0, 0, img_size_.width / 2, img_size_.height);
  cv::Rect right_roi(img_size_.width / 2, 0, img_size_.width / 2,
                     img_size_.height);
  cv::Mat halfImg_left = img(left_roi).clone();
  cv::Mat halfImg_right = img(right_roi).clone();
  std::vector<cv::Point2d> conner_pts1, conner_pts2;
  detectConner(halfImg_left, extra_CBSize_, conner_pts1);
  if (conner_pts1.empty()) {
    return;
  }
  reorderConner(conner_pts1);
  detectConner(halfImg_right, extra_CBSize_, conner_pts2);
  if (conner_pts2.empty()) {
    return;
  }
  reorderConner(conner_pts2);
  for (size_t inum = 0; inum < conner_pts2.size(); ++inum) {
    conner_pts2[inum].x += right_roi.x;
  }

  if (0 == chn) {
    allConners.insert(allConners.end(), conner_pts1.begin(), conner_pts1.end());
    allConners.insert(allConners.end(), conner_pts2.begin(), conner_pts2.end());
  } else {
    for (int inum = 0; inum < (int)conner_pts1.size() / 2; ++inum) {
      std::swap(conner_pts1[extra_CBSize_.area() - 1 - inum],
                conner_pts1[inum]);
      std::swap(conner_pts2[extra_CBSize_.area() - 1 - inum],
                conner_pts2[inum]);
    }
    allConners.insert(allConners.end(), conner_pts2.begin(), conner_pts2.end());
    allConners.insert(allConners.end(), conner_pts1.begin(), conner_pts1.end());
  }
}

int AvmExtrinsCalib::refineExtrins(int chn, const cv::Mat &img, cv::Mat &rvec,
                                   cv::Mat &tvec, cv::Mat &H,
                                   std::vector<cv::Point3d> &world_pts,
                                   std::vector<cv::Point2d> &pix_pts) {
  int ret_val = AVM_NORMAL_STATE;
  // 前后左右小棋盘格在逆时针顺序存储下的索引
  std::vector<int> indexs = {0, 3, 1, 2, 0, 1, 3, 2};
  // 原图检测有一定的误差与速度改善，但是会带来角点检测率偏低的情况，
  //  暂时保留鸟瞰图检测额外角点的方式
  if (chn > -1) {
    cv::Mat bev_img;
    cv::Rect assistant_rois[2];
    remapping(chn, rvec, tvec, img, bev_img, assistant_rois);
    std::vector<cv::Point2d> extra_cbPoints;
    getAllConner(chn, bev_img, assistant_rois, extra_cbPoints);

    if (extra_cbPoints.empty()) {
      ret_val = AVM_ERROR_CONNER_NOT_DETECTED;
    } else {
      // 将棋盘格角点逆时针存储，例如：前面是第0个和第三个小棋盘格角点放在中间棋盘格旁边
      for (size_t inum = 0; inum < extra_cbPoints.size(); ++inum) {
        cv::Point2d pix_pt;
        cv::Point3d pw;
        bevPix2Vehicle(extra_cbPoints[inum], pw);
        project(chn, rvec, tvec, pw, pix_pt);
        pix_pts.push_back(pix_pt);
      }
    }
  } else {
    std::vector<cv::Point2d> extra_pix;
    getFrontRearExtraConner(chn, img, extra_pix);
    pix_pts.insert(pix_pts.end(), extra_pix.begin(), extra_pix.end());
  }
  for (int i = 0; i < 2; ++i) {
    for (int inum = 0; inum < extra_CBSize_.area(); ++inum) {
      world_pts.push_back(extra_CBPoints_[indexs[chn * 2 + i]][inum]);
    }
  }
  if (world_pts.size() != pix_pts.size()) {
    ret_val = AVM_ERROR_CONNER_NOT_DETECTED;
  } else {
    calcExtrins(chn, world_pts, pix_pts, rvec, tvec, H, true);
  }

  return ret_val;
}

void AvmExtrinsCalib::bevPix2Vehicle(const cv::Point2d &bev_pt,
                                     cv::Point3d &vehicle_pt) {
  CarInfo car_info = avm_layout_->getCarInfo();

  float sight_width = SIGHT_WIDTH + car_info.car_width() / 2.0f;
  float sight_height = (sight_width / BIRD_VIEW_WIDTH * BIRD_VIEW_HEIGHT);

  float t_offset = -car_info.car_length() / 2 + car_info.car_front_overhang() +
                   car_info.car_wheelbase();

  float x_start = (float)(sight_height);
  float y_start = (float)(sight_width);
  float x_stride = (float)sight_height / (float)(BIRD_VIEW_HEIGHT / 2);
  float y_stride = (float)sight_width / (float)(BIRD_VIEW_WIDTH / 2);

  vehicle_pt = cv::Point3d(x_start - bev_pt.y * x_stride,
                           y_start - bev_pt.x * y_stride, 0.0);
  vehicle_pt.x += t_offset;
}
} // namespace avm
} // namespace minieye