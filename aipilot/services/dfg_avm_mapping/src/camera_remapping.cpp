//
// Created by minieye on 2022/6/23.
//
#include "aipilot/services/dfg_avm_mapping/src/camera_remapping.h"

#include "aipilot/services/dfg_avm_mapping/src/avm_parse.h"
#include "glog/logging.h"
#include "msg/precompiler/camera.pb.h"

// camera model
#include "aipilot/services/dfg_avm_mapping/camera/cmei_model.h"
#include "aipilot/services/dfg_avm_mapping/camera/davide_scaramuzza_model.h"
#include "aipilot/services/dfg_avm_mapping/camera/fisheye_model.h"

namespace minieye {
namespace avm {

int32_t warpPoint(const cv::Mat &h, const cv::Point2f &pt, cv::Point2f &outPt);

bool AvmRemapping::initCamera(const std::vector<std::string> &cam_configs_paths,
                              const std::string &car_info_str) {
  bool ret = false;

  for (uint32_t inum = 0; inum < cam_configs_paths.size(); ++inum) {
    CameraParam camera_param;
    std::shared_ptr<base::BaseCameraDistortionModel> camera_model;
    ret = parseIntrinsJson(cam_configs_paths[inum], &camera_param);

    int32_t ret_code = getCameraModel(camera_param, camera_model);
    if (AVM_NORMAL_STATE != ret_code) {
      ret = false;
      break;
    }
    uint32_t camera_id = camera_param.camera_id();
    uint32_t ch = (uint32_t)std::log2(camera_id);
    camera_models_.insert(
        std::pair<uint32_t, std::shared_ptr<base::BaseCameraDistortionModel>>(
            ch, camera_model));
    img_size_ =
        cv::Size(camera_param.image_width(), camera_param.image_height());
    camera_params_[ch] = camera_param;
  }
  if (camera_params_.size() != 4) {
    LOG(ERROR) << "fail to load intrins";
    ret = false;
  }
  if (ret) {
    CarInfo car_info;
    ret = parseCarInfoJson(car_info_str, &car_info);
    if (ret) {
      car_info_ = car_info;
    }
    getKRT();
  }

  return ret;
}

bool AvmRemapping::initCamera(const std::vector<CameraParam> &camera_params,
                              const CarInfo &car_info) {
  bool ret = false;

  for (uint32_t inum = 0; inum < camera_params.size(); ++inum) {
    std::shared_ptr<base::BaseCameraDistortionModel> camera_model;

    int32_t ret_code = getCameraModel(camera_params[inum], camera_model);
    if (AVM_NORMAL_STATE != ret_code) {
      ret = false;
      break;
    }
    uint32_t camera_id = camera_params[inum].camera_id();
    uint32_t ch = (uint32_t)std::log2(camera_id);
    camera_models_.insert(
        std::pair<uint32_t, std::shared_ptr<base::BaseCameraDistortionModel>>(
            ch, camera_model));
    img_size_ = cv::Size(camera_params[inum].image_width(),
                         camera_params[inum].image_height());
    camera_params_[ch] = camera_params[inum];
  }
  if (camera_params_.size() != 4) {
    LOG(ERROR) << "fail to load intrins";
    ret = false;
  } else {
    ret = true;
    car_info_ = car_info;
    getKRT();
  }

  return ret;
}

int32_t AvmRemapping::getCameraModel(
    const CameraParam &camera_param,
    std::shared_ptr<base::BaseCameraDistortionModel> &camera_model) {
  AvmErrorCode ret_code = AVM_NORMAL_STATE;
  float fu = camera_param.focal_u();
  float fv = camera_param.focal_v();
  float cu = camera_param.cu();
  float cv = camera_param.cv();
  int32_t image_width = camera_param.image_width();
  int32_t image_height = camera_param.image_height();
  cv::Mat K = (cv::Mat_<double>(3, 3) << fu, 0, cu, 0.0, fv, cv, 0.0, 0.0, 1.0);

  switch (camera_param.prj_model()) {
  case FISHEYE: {
    camera_model = std::make_shared<base::FisheyeCameraDistortionModel>();
    cv::Mat D = (cv::Mat_<double>(1, 4) << camera_param.distort_coeffs(0),
                 camera_param.distort_coeffs(1), camera_param.distort_coeffs(2),
                 camera_param.distort_coeffs(3));
    camera_model->set_params(image_width, image_height, K, D);
  } break;
  case DAVIDE_SCARAMUZZA: {
    camera_model = std::make_shared<base::DavideScaramCameraDistortionModel>();
    int32_t size = camera_param.distort_coeffs().size();

    std::vector<double> dc(camera_param.distort_coeffs().begin(),
                           camera_param.distort_coeffs().end());

    cv::Mat D(1, size, CV_64F);
    memcpy(D.data, dc.data(), sizeof(double) * size);
    D.convertTo(D, CV_32F);

    const uint32_t invploy_length = size - 9;
    // 畸变参数包括中心点
    cv::Mat distort_params(1, size, CV_64F);
    uint32_t offset = 0;
    memcpy(distort_params.data, dc.data(), 9 * sizeof(double));
    offset += 9 * sizeof(double);
    memcpy(distort_params.data + offset, &dc[9],
           sizeof(double) * (invploy_length));
    camera_model->set_params(image_width, image_height, (cv::Mat)K,
                             (cv::Mat)distort_params);
  } break;
  case MEI: {
    camera_model = std::make_shared<base::CMeiCameraDistortionModel>();
    cv::Mat D =
        (cv::Mat_<double>(1, 5) << camera_param.distort_coeffs(1),
         camera_param.distort_coeffs(2), camera_param.distort_coeffs(3),
         camera_param.distort_coeffs(4), camera_param.distort_coeffs(5));
    K.at<double>(0, 1) = camera_param.distort_coeffs(0);
    camera_model->set_params(image_width, image_height, K, D);
  } break;
  default: {
    LOG(ERROR) << "unsupport project model";
    ret_code = AVM_ERROR_ABNORMAL_INTRINS;
  } break;
  }
  return ret_code;
}

int32_t AvmRemapping::getQuadrant(const cv::Point3f &in_world_pt) {
  int32_t quard_num = 0;
  // 平移至车身中心
  cv::Point3d world_pt = in_world_pt;
  world_pt.x =
      world_pt.x - (car_info_.car_front_overhang() + car_info_.car_wheelbase() -
                    car_info_.car_length() / 2.0);
  if (world_pt.x > 0 && world_pt.y < 0) {
    quard_num = 0;
  } else if (world_pt.x > 0 && world_pt.y > 0) {
    quard_num = 1;
  } else if (world_pt.x < 0 && world_pt.y > 0) {
    quard_num = 2;
  } else if (world_pt.x < 0 && world_pt.y < 0) {
    quard_num = 3;
  } else if (world_pt.x > 0 && world_pt.y == 0) {
    quard_num = 4;
  } else if (world_pt.x == 0 && world_pt.y > 0) {
    quard_num = 5;
  } else if (world_pt.x < 0 && world_pt.y == 0) {
    quard_num = 6;
  } else if (world_pt.x == 0 && world_pt.y < 0) {
    quard_num = 7;
  }
  return quard_num;
}

int32_t AvmRemapping::getQuadrantBlendImg(int32_t quard_num, int32_t &left_ch,
                                          int32_t &right_ch) {
  // 四个象限，每个象限逆时针旋转，分左右图像
  switch (quard_num) {
  case 0: {
    left_ch = 0;
    right_ch = 3;
  } break;
  case 1: {
    left_ch = 2;
    right_ch = 0;
  } break;
  case 2: {
    left_ch = 1;
    right_ch = 2;
  } break;
  case 3: {
    left_ch = 3;
    right_ch = 1;
  } break;
  case 4: {
    left_ch = 0;
    right_ch = 0;
  } break;
  case 5: {
    left_ch = 2;
    right_ch = 2;
  } break;
  case 6: {
    left_ch = 1;
    right_ch = 1;
  } break;
  case 7: {
    left_ch = 3;
    right_ch = 3;
  } break;
  }
  return 0;
}

int32_t AvmRemapping::cal_blend(int32_t quard_num, int32_t angle_offset,
                                const cv::Point3d &world_pt, float &weight) {
  const double SIGHT_W = 25;
  const double SIGHT_H = 25;
  double sight_w = SIGHT_W + car_info_.car_width() / 2.0;
  double sight_h = SIGHT_H + car_info_.car_length() / 2.0;
  // 从第0个象限逆时针存储
  std::vector<cv::Point2d> outer_conner = {
      cv::Point2d(sight_h, -sight_w), cv::Point2d(sight_h, sight_w),
      cv::Point2d(-sight_h, sight_w), cv::Point2d(-sight_h, -sight_w)};
  std::vector<cv::Point2d> inter_conner = {
      cv::Point2d(car_info_.car_length() / 2.0, -car_info_.car_width() / 2.0),
      cv::Point2d(car_info_.car_length() / 2.0, car_info_.car_width() / 2.0),
      cv::Point2d(-car_info_.car_length() / 2.0, car_info_.car_width() / 2.0),
      cv::Point2d(-car_info_.car_length() / 2.0, -car_info_.car_width() / 2.0)};

  cv::Vec2d conner_vec =
      cv::Vec2d(outer_conner[quard_num].x - inter_conner[quard_num].x,
                outer_conner[quard_num].y - inter_conner[quard_num].y);

  // 原始的坐标点是以车后轮作为中心，需将此处的世界坐标点平移至车身中心
  cv::Point3d new_world_pt = world_pt;
  new_world_pt.x = new_world_pt.x -
                   (car_info_.car_front_overhang() + car_info_.car_wheelbase() -
                    car_info_.car_length() / 2.0);

  cv::Vec2d pt_vec = cv::Vec2d(new_world_pt.x - inter_conner[quard_num].x,
                               new_world_pt.y - inter_conner[quard_num].y);

  int32_t polarity;
  double cross_res = conner_vec[0] * pt_vec[1] - conner_vec[1] * pt_vec[0];
  polarity = cross_res > 0 ? 1 : cross_res < 0 ? -1 : 0;
  double cos_degree =
      conner_vec.dot(pt_vec) / (cv::norm(conner_vec) * cv::norm(pt_vec));
  cos_degree = std::acos(cos_degree) * 180.0 / CV_PI;
  cos_degree *= polarity;
  if (cos_degree > (double)angle_offset) {
    weight = 1;
  } else if (cos_degree < -(double)angle_offset) {
    weight = 0;
  } else {
    weight = ((float)cos_degree + (float)angle_offset) / (2.0f * angle_offset);
  }
  return 0;
}

int32_t AvmRemapping::distort(uint32_t camera_direction,
                              const cv::Point2f &undistort_pt,
                              cv::Point2f &distort_pt) {
  int chn = (int)std::log2(camera_direction);
  cv::Point2d distort_pix = camera_models_[chn]->distort(undistort_pt);

  distort_pt.x = distort_pix.x;
  distort_pt.y = distort_pix.y;
  return 0;
}

int32_t AvmRemapping::undistort(uint32_t camera_direction,
                                const cv::Point2f &pt,
                                cv::Point2f &undistort_pt) {
  int chn = (int)std::log2(camera_direction);
  cv::Point2d undistort_pix = camera_models_[chn]->undistort(pt);

  undistort_pt.x = undistort_pix.x;
  undistort_pt.y = undistort_pix.y;
  return 0;
}

void AvmRemapping::getKRT() {
  Hs_.clear();
  Hs_.resize(camera_params_.size());
  Ks_.clear();
  Ks_.resize(camera_params_.size());
  RTs_.clear();
  RTs_.resize(camera_params_.size());
  for (uint32_t inum = 0; inum < camera_params_.size(); ++inum) {
    uint32_t chn = std::log2(camera_params_[inum].camera_id());
    cv::Mat H = cv::Mat(3, 3, CV_32F);
    cv::Mat k = (cv::Mat_<float>(3, 3) << camera_params_[chn].focal_u(), 0,
                 camera_params_[chn].cu(), 0, camera_params_[chn].focal_v(),
                 camera_params_[chn].cv(), 0, 0, 1);
    if (MEI == camera_params_[chn].prj_model()) {
      k.at<float>(0, 1) = camera_params_[chn].distort_coeffs(0);
    }
    cv::Mat rvec = (cv::Mat_<float>(1, 3) << camera_params_[chn].pitch(),
                    camera_params_[chn].yaw(), camera_params_[chn].roll());
    cv::Mat tvec = (cv::Mat_<float>(1, 3) << camera_params_[chn].pos(0),
                    camera_params_[chn].pos(1), camera_params_[chn].pos(2));
    cv::Mat rotateMat;
    cv::Rodrigues(rvec, rotateMat);
    rotateMat.convertTo(H, CV_32F);
    cv::Mat rt(3, 4, CV_32F);
    H.copyTo(rt(cv::Range(0, 3), cv::Range(0, 3)));
    float *tvec_ptr = tvec.ptr<float>(0);
    for (uint32_t irow = 0; irow < 3; ++irow) {
      H.ptr<float>(irow)[2] = tvec_ptr[irow];
      rt.ptr<float>(irow)[3] = tvec_ptr[irow];
    }
    H = k * H;
    H.convertTo(H, CV_64F);
    k.convertTo(k, CV_64F);
    rt.convertTo(rt, CV_64F);
    Hs_[chn] = H.clone();
    Ks_[chn] = k.clone();
    RTs_[chn] = rt.clone();
  }
}

int32_t AvmRemapping::unproject(uint32_t camera_direction,
                                const cv::Point2d &pix_pt,
                                cv::Point3d &world_pt) {
  int32_t ret = 0;
  uint32_t chn = std::log2(camera_direction);
  if (chn < 0 || chn > 3) {
    ret = -1;
  } else {
    cv::Mat h = Hs_[chn];
    cv::Point2f undist_pt;
    undistort(camera_direction, pix_pt, undist_pt);

    cv::Point2f pt;
    warpPoint(h.inv(), undist_pt, pt);
    world_pt.x = pt.x;
    world_pt.y = pt.y;
    world_pt.z = 0.0f;
  }

  return ret;
}

int32_t AvmRemapping::project(const cv::Point3d &pw, SrvLut &srv_lut) {
  int32_t quard_num = getQuadrant(pw);
  int left_ch = 0, right_ch = 0;
  getQuadrantBlendImg(quard_num, left_ch, right_ch);

  cv::Vec4d world_vec = cv::Vec4d(pw.x, pw.y, pw.z, 1);
  cv::Mat h_left = RTs_[left_ch];
  cv::Mat h_right = RTs_[right_ch];
  cv::Vec3d camera_pt1 = (cv::Matx34d)h_left * world_vec;
  cv::Vec3d camera_pt2 = (cv::Matx34d)h_right * world_vec;
  if (std::abs(camera_pt1[2]) > 0 && std::abs(camera_pt2[2]) > 0) {
    camera_pt1 /= camera_pt1[2];
    camera_pt2 /= camera_pt2[2];

    cv::Point3d left_pw(camera_pt1[0], camera_pt1[1], camera_pt1[2]);
    cv::Point3d right_pw(camera_pt2[0], camera_pt2[1], camera_pt2[2]);
    cv::Point2d left_pix_pt = camera_models_[left_ch]->Project(left_pw);
    cv::Point2d right_pix_pt = camera_models_[right_ch]->Project(right_pw);

    left_pix_pt.x = left_pix_pt.x > img_size_.width - 2 ? img_size_.width - 1
                                                        : left_pix_pt.x;
    left_pix_pt.y = left_pix_pt.y > img_size_.height - 2 ? img_size_.height - 1
                                                         : left_pix_pt.y;
    left_pix_pt.x = left_pix_pt.x < 0 ? 0 : left_pix_pt.x;
    left_pix_pt.y = left_pix_pt.y < 0 ? 0 : left_pix_pt.y;

    right_pix_pt.x = right_pix_pt.x > img_size_.width - 2 ? img_size_.width - 1
                                                          : right_pix_pt.x;
    right_pix_pt.y = right_pix_pt.y > img_size_.height - 2
                         ? img_size_.height - 1
                         : right_pix_pt.y;
    right_pix_pt.x = right_pix_pt.x < 0 ? 0 : right_pix_pt.x;
    right_pix_pt.y = right_pix_pt.y < 0 ? 0 : right_pix_pt.y;

    float left_w = 1.0f;
    if (quard_num < 4) {
      const float angle_offset = 20.0f;
      cal_blend(quard_num, angle_offset, pw, left_w);
    }

    srv_lut.x = pw.x;
    srv_lut.y = pw.y;
    srv_lut.z = pw.z;
    srv_lut.camera_id1 = 1u << left_ch;
    srv_lut.u1 = left_pix_pt.x;
    srv_lut.v1 = left_pix_pt.y;
    srv_lut.w1 = left_w;
    srv_lut.camera_id2 = 1u << right_ch;
    srv_lut.u2 = right_pix_pt.x;
    srv_lut.v2 = right_pix_pt.y;
    srv_lut.w2 = 1.0f - left_w;

    if (isInCar(pw)) {
      srv_lut.w1 = 0;
      srv_lut.w2 = 0;
    }
  } else {
    LOG(ERROR) << "abnormal world point";
  }

  return 0;
}

bool AvmRemapping::isInCar(const cv::Point3f &in_world_pt) {
  // 原始的坐标点是以车后轮作为中心，需将此处的世界坐标点平移至车身中心
  cv::Point3d world_pt = in_world_pt;
  world_pt.x -= car_info_.car_front_overhang() + car_info_.car_wheelbase() -
                car_info_.car_length() / 2.0;
  // 车辆边界
  float up = car_info_.car_length() / 2.0f + car_info_.car_front_blend();
  float bottom = -(car_info_.car_length() / 2.0f + car_info_.car_rear_blend());
  float left = car_info_.car_width() / 2.0f + car_info_.car_left_blend();
  float right = -(car_info_.car_width() / 2.0f + car_info_.car_right_blend());
  bool flag = false;
  flag = !(world_pt.y <= right || world_pt.x >= up || world_pt.x <= bottom ||
           world_pt.y >= left);

  return flag;
}

bool AvmRemapping::initCamera(
    const std::vector<std::string> &cam_configs_paths) {
  bool ret = true;
  for (uint32_t inum = 0; inum < cam_configs_paths.size(); ++inum) {
    CameraParam camera_param;
    std::shared_ptr<base::BaseCameraDistortionModel> camera_model;
    ret = parseIntrinsJson(cam_configs_paths[inum], &camera_param);

    int32_t ret_code = getCameraModel(camera_param, camera_model);
    if (AVM_NORMAL_STATE != ret_code) {
      ret = false;
      break;
    }
    uint32_t camera_id = camera_param.camera_id();
    uint32_t ch = (uint32_t)std::log2(camera_id);
    camera_models_.insert(
        std::pair<uint32_t, std::shared_ptr<base::BaseCameraDistortionModel>>(
            ch, camera_model));
    img_size_ =
        cv::Size(camera_param.image_width(), camera_param.image_height());
    camera_params_[ch] = camera_param;
  }
  if (camera_params_.size() != 4) {
    LOG(ERROR) << "fail to load intrins";
    ret = false;
  }
  return ret;
}

int32_t AvmRemapping::project(int32_t camera_id, const cv::Point3d &pw,
                              cv::Point2d &pix_pt) {
  int chn = (int)std::log2(camera_id);
  cv::Vec4d world_vec = cv::Vec4d(pw.x, pw.y, pw.z, 1);
  cv::Mat rt = RTs_[chn];
  cv::Vec3d camera_pt1 = (cv::Matx34d)rt * world_vec;

  if (std::abs(camera_pt1[2]) > 0) {
    camera_pt1 /= camera_pt1[2];

    cv::Point3d unit_pw(camera_pt1[0], camera_pt1[1], camera_pt1[2]);
    pix_pt = camera_models_[chn]->Project(unit_pw);

    pix_pt.x = pix_pt.x > img_size_.width - 2 ? img_size_.width - 1 : pix_pt.x;
    pix_pt.y =
        pix_pt.y > img_size_.height - 2 ? img_size_.height - 1 : pix_pt.y;
    pix_pt.x = pix_pt.x < 0 ? 0 : pix_pt.x;
    pix_pt.y = pix_pt.y < 0 ? 0 : pix_pt.y;
  }
  return 0;
}

int32_t warpPoint(const cv::Mat &h, const cv::Point2f &pt, cv::Point2f &outPt) {
  int32_t ret = 0;
  const double *h0 = h.ptr<double>(0);
  const double *h1 = h.ptr<double>(1);
  const double *h2 = h.ptr<double>(2);

  double x = h0[0] * pt.x + h0[1] * pt.y + h0[2];
  double y = h1[0] * pt.x + h1[1] * pt.y + h1[2];
  double n = h2[0] * pt.x + h2[1] * pt.y + h2[2];
  n = 1.0f / n;
  outPt.x = x * n;
  outPt.y = y * n;
  return ret;
}

} // namespace avm
} // namespace minieye