//
// Created by minieye on 2022/6/22.
//
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "json.hpp"
#include <fstream>

#include "aipilot/services/dfg_avm_calib/src/jx65_layout.h"
#include "aipilot/services/dfg_avm_mapping/src/avm_parse.h"

DEFINE_double(CW1, 3.8, "cw1");
DEFINE_double(CW2, 3.0, "cw2");
DEFINE_double(CW3, 2.4, "cw3");
DEFINE_double(CW4, 1.0, "cw4");
DEFINE_double(CW5, 0.8, "cw5");

DEFINE_int32(extra_checkerboard_horiz_num, 3, "extra checker board horiz num");
DEFINE_int32(extra_checkerboard_vert_num, 3, "extra checker board vert num");

namespace minieye {
namespace avm {

int JX65Layout::initLayout(const std::string &extrins_file,
                           const std::string &car_info_json) {
  int ret_code = AVM_NORMAL_STATE;
  bool flag = parseExtrins(extrins_file);
  if (flag) {
    flag = parseCarInfoJson(car_info_json, &car_info_);
  }
  extra_checker_info_.CW1 = static_cast<float>(FLAGS_CW1);
  extra_checker_info_.CW2 = static_cast<float>(FLAGS_CW2);
  extra_checker_info_.CW3 = static_cast<float>(FLAGS_CW3);
  extra_checker_info_.CW4 = static_cast<float>(FLAGS_CW4);
  extra_checker_info_.CW5 = static_cast<float>(FLAGS_CW5);

  if (!flag) {
    ret_code = AVM_ERROR_ABNORMAL_EXTRINS;
  }

  minD_ = FLAGS_CW4 - FLAGS_CW5;
  maxD_ = FLAGS_CW2 - FLAGS_CW3;
  return ret_code;
}

bool JX65Layout::parseExtrins(const std::string &extrins_file) {
  bool ret = true;
  nlohmann::json config_json;
  std::ifstream fin(extrins_file);
  if (!fin.is_open()) {
    LOG(ERROR) << "failed to load extrins_file file : " << extrins_file;
    ret = false;
  } else {
    fin >> config_json;
    layout_type_ = config_json["layout"];
    if (1 == layout_type_ || 2 == layout_type_) {
      nlohmann::json layout_json = config_json["jx65_type"];
      checker_info_.checker_dw = layout_json["checker_dw"];
      checker_info_.checker_dh = layout_json["checker_dh"];
      checker_info_.vert_num = layout_json["vert_num"];
      checker_info_.horize_num = layout_json["horize_num"];

      layout_param_.w = layout_json["w"];
      layout_param_.h = layout_json["h"];
      layout_param_.d = layout_json["d"];
      layout_param_.v = layout_json["v"];
      layout_param_.w1 = layout_json["w1"];
      layout_param_.w2 = layout_json["w2"];
    } else {
      LOG(ERROR) << "failed to load common layout params : " << extrins_file;
      ret = false;
    }
  }
  fin.close();
  return ret;
}

int JX65Layout::getLayoutWorldPts(
    std::vector<std::vector<cv::Point3d>> &world_pts) {
  world_pts.clear();
  for (uint32_t inum = 0; inum < 4; ++inum) {
    uint32_t camera_id = 1u << inum;
    std::vector<cv::Point3d> plane_pt;
    getWorldPt(camera_id, plane_pt);
    world_pts.emplace_back(plane_pt);
  }
  return 0;
}

int JX65Layout::getWorldPt(uint32_t camera_id,
                           std::vector<cv::Point3d> &world_pt) {
  std::vector<cv::Point3d> plane_pts;
  int32_t chn = (int)std::log2(camera_id);
  switch (chn) {
  case 0:
    getFrontPt(plane_pts);
    break;
  case 1:
    getRearPt(plane_pts);
    break;
  case 2:
    getLeftPt(plane_pts);
    break;
  case 3:
    getRightPt(plane_pts);
    break;
  }
  // 将各个方向的坐标移到车身后轴中心
  if (!plane_pts.empty()) {
    translate(plane_pts, world_pt);
  }
  return 0;
}

int JX65Layout::getCheckerBoardConnerSize(cv::Size &conner_size) {
  conner_size = cv::Size(checker_info_.horize_num, checker_info_.vert_num);
  return 0;
}

void JX65Layout::getFrontPt(std::vector<cv::Point3d> &front_world_pt) {
  front_world_pt.clear();
  int32_t horiz_num = checker_info_.horize_num;
  int32_t vert_num = checker_info_.vert_num;
  int32_t exrtra_horiz_num = FLAGS_extra_checkerboard_horiz_num;
  int32_t extra_vert_num = FLAGS_extra_checkerboard_vert_num;
  auto dw = checker_info_.checker_dw;
  auto dh = checker_info_.checker_dh;
  float w1 = layout_param_.w1;
  // 确定左上角点坐标，根据棋盘格大小确定别的角点坐标
  // 北西天坐标系，x为前进方向，y为前进方向左边
  double y0 = -(w1 + (exrtra_horiz_num + 1) * dw + minD_ + dw);
  double x0 = -dh;
  for (int32_t ivert = 0; ivert < vert_num; ivert++) {
    for (int32_t ihoriz = 0; ihoriz < horiz_num; ihoriz++) {
      double x = x0 - ivert * dh;
      double y = y0 - ihoriz * dw;
      cv::Point3d pt = cv::Point3d(x, y, 0.0);
      front_world_pt.push_back(pt);
    }
  }
}

void JX65Layout::getLeftPt(std::vector<cv::Point3d> &left_world_pt) {
  left_world_pt.clear();
  auto v = layout_param_.v;
  int32_t horiz_num = checker_info_.horize_num;
  int32_t vert_num = checker_info_.vert_num;
  auto dw = checker_info_.checker_dw;
  auto dh = checker_info_.checker_dh;

  float y0 = -dh;
  float x0 = -(horiz_num * dw + v + (vert_num + 1) * dh);
  for (int32_t ivert = 0; ivert < vert_num; ivert++) {
    for (int32_t ihoriz = 0; ihoriz < horiz_num; ihoriz++) {
      float x = x0 + ihoriz * dw;
      float y = y0 - ivert * dh;
      cv::Point3d pt = cv::Point3d(x, y, 0.0);
      left_world_pt.push_back(pt);
    }
  }
}

void JX65Layout::getRightPt(std::vector<cv::Point3d> &right_world_pt) {
  right_world_pt.clear();
  auto v = (float)layout_param_.v;
  auto w = (float)layout_param_.w;
  int32_t horiz_num = checker_info_.horize_num;
  int32_t vert_num = checker_info_.vert_num;
  auto dw = (float)checker_info_.checker_dw;
  auto dh = (float)checker_info_.checker_dh;

  float x0 = -(v + (vert_num + 1) * dh + dw);
  float y0 = -(w + (vert_num + 1) * dh + vert_num * dh);

  for (int32_t ivert = 0; ivert < vert_num; ++ivert) {
    for (int32_t ihoriz = 0; ihoriz < horiz_num; ++ihoriz) {
      float x = x0 - ihoriz * dw;
      float y = y0 + ivert * dh;
      cv::Point3d pt = cv::Point3d(x, y, 0.0);
      right_world_pt.push_back(pt);
    }
  }
}

void JX65Layout::getRearPt(std::vector<cv::Point3d> &rear_world_pt) {
  rear_world_pt.clear();
  auto w = (float)layout_param_.w;
  auto h = (float)layout_param_.h;
  auto w1 = layout_param_.w1;
  int32_t exrtra_horiz_num = FLAGS_extra_checkerboard_horiz_num;
  int32_t extra_vert_num = FLAGS_extra_checkerboard_vert_num;
  int32_t horiz_num = checker_info_.horize_num;
  int32_t vert_num = checker_info_.vert_num;
  auto dw = (float)checker_info_.checker_dw;
  auto dh = (float)checker_info_.checker_dh;

  float x0 = -(h + (vert_num + 1) * dh + vert_num * dh);
  float y0 = -(w1 + (exrtra_horiz_num + 1) * dw + maxD_ + horiz_num * dw);

  for (int32_t ivert = 0; ivert < vert_num; ++ivert) {
    for (int32_t ihoriz = 0; ihoriz < horiz_num; ++ihoriz) {
      float x = x0 + ivert * dh;
      float y = y0 + ihoriz * dw;
      cv::Point3d pt = cv::Point3d(x, y, 0.0);
      rear_world_pt.push_back(pt);
    }
  }
}

void JX65Layout::translate(const std::vector<cv::Point3d> &world_pts,
                           std::vector<cv::Point3d> &new_world_pts) {
  auto d = (float)layout_param_.d;
  int32_t vert_num = checker_info_.vert_num;
  auto dh = (float)checker_info_.checker_dh;
  auto w2 = layout_param_.w2;

  float car_width = car_info_.car_width();
  float car_wheelbase = car_info_.car_wheelbase();
  float car_front_overhang = car_info_.car_front_overhang();

  float y0 = w2 + car_width / 2.0;
  float x0 = (car_front_overhang + car_wheelbase) + (vert_num + 1) * dh + d;

  new_world_pts.resize(world_pts.size());
  for (uint32_t inum = 0; inum < world_pts.size(); ++inum) {
    new_world_pts[inum].x = world_pts[inum].x + x0;
    new_world_pts[inum].y = world_pts[inum].y + y0;
    new_world_pts[inum].z = world_pts[inum].z;
  }
}

int JX65Layout::getExtraCheckerBoardInfo(
    cv::Size &extra_board_size,
    std::vector<std::vector<cv::Point3d>> &extra_conner) {
  extra_board_size = cv::Size(FLAGS_extra_checkerboard_horiz_num,
                              FLAGS_extra_checkerboard_vert_num);
  auto w = (float)layout_param_.w;
  auto h = (float)layout_param_.h;
  auto w1 = layout_param_.w1;
  int32_t exrtra_horiz_num = FLAGS_extra_checkerboard_horiz_num;
  int32_t extra_vert_num = FLAGS_extra_checkerboard_vert_num;
  int32_t horiz_num = checker_info_.horize_num;
  int32_t vert_num = checker_info_.vert_num;
  auto dw = (float)checker_info_.checker_dw;
  auto dh = (float)checker_info_.checker_dh;

  // 小棋盘格按照左上起始位置进行角点顺序存储
  // front-left conner
  float x0 = -dh * 2;
  float y0 = -(w1 + dw);
  std::vector<cv::Point3d> front_left_conner;

  // left-rear
  float x1 = -(h + (vert_num + 1) * dh + dh);
  float y1 = -(w1 + dw);
  std::vector<cv::Point3d> left_rear_conner;

  // rear-right
  float x2 = -(h + (vert_num + 1) * dh + dh);
  float y2 = -(w + (vert_num + 1) * dh + dh);
  std::vector<cv::Point3d> rear_right_conner;

  // right-front
  float x3 = -(dh * 2);
  float y3 = -(w + (vert_num + 1) * dh + dh);
  std::vector<cv::Point3d> right_front_conner;

  for (int irow = 0; irow < exrtra_horiz_num; ++irow) {
    for (int icol = 0; icol < extra_vert_num; ++icol) {
      cv::Point3d pw1(x0 - irow * dh, y0 - icol * dw, 0);
      front_left_conner.push_back(pw1);

      cv::Point3d pw2(x1 - irow * dh, y1 - icol * dw, 0);
      left_rear_conner.push_back(pw2);

      cv::Point3d pw3(x2 - irow * dh, y2 - icol * dw, 0);
      rear_right_conner.push_back(pw3);

      cv::Point3d pw4(x3 - irow * dh, y3 - icol * dw, 0);
      right_front_conner.push_back(pw4);
    }
  }

  extra_conner.clear();
  extra_conner.resize(4);
  // 将各个方向的坐标移到车身后轴中心
  translate(front_left_conner, extra_conner[0]);
  translate(left_rear_conner, extra_conner[1]);
  translate(rear_right_conner, extra_conner[2]);
  translate(right_front_conner, extra_conner[3]);
  return 0;
}
} // namespace avm
} // namespace minieye