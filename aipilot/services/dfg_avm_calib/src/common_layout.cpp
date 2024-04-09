//
// Created by minieye on 2022/6/21.
//
#include "glog/logging.h"
#include "json.hpp"

#include "aipilot/services/dfg_avm_mapping/src/avm_parse.h"
#include "aipilot/services/dfg_avm_calib/src/common_layout.h"

namespace minieye {
namespace avm {

int CommonLayout::initLayout(const std::string &extrins_file,
                             const std::string &car_info_json) {
  int ret_code = AVM_NORMAL_STATE;
  bool flag = parseExtrins(extrins_file);
  if (flag) {
    flag = parseCarInfoJson(car_info_json, &car_info_);
  }
  if (!flag) {
    ret_code = AVM_ERROR_ABNORMAL_EXTRINS;
  }
  return ret_code;
}

bool CommonLayout::parseExtrins(const std::string &extrins_file) {
  bool ret = true;
  nlohmann::json config_json;
  std::ifstream fin(extrins_file);
  if (!fin.is_open()) {
    LOG(ERROR) << "failed to load extrins_file file : " << extrins_file;
    ret = false;
  } else {
    fin >> config_json;
    layout_type_ = config_json["layout"];
    if (0 == layout_type_) {
      nlohmann::json layout_json = config_json["common_type"];
      checker_info_.checker_dw = layout_json["checker_dw"];
      checker_info_.checker_dh = layout_json["checker_dh"];
      checker_info_.vert_num = layout_json["vert_num"];
      checker_info_.horize_num = layout_json["horize_num"];

      layout_param_.w = layout_json["w"];
      layout_param_.h = layout_json["h"];
      layout_param_.d = layout_json["d"];
      layout_param_.v = layout_json["v"];
    } else {
      LOG(ERROR) << "failed to load common layout params : " << extrins_file;
      ret = false;
    }
  }
  fin.close();
  return ret;
}

int CommonLayout::getLayoutWorldPts(
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

int CommonLayout::getWorldPt(uint32_t camera_id,
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
  // 将各个方向的坐标移到车身中心坐标
  if (!plane_pts.empty()) {
    translate(plane_pts, world_pt);
  }
  return 0;
}

void CommonLayout::getFrontPt(std::vector<cv::Point3d> &front_world_pt) {
  front_world_pt.clear();
  auto w = layout_param_.w;
  int32_t horiz_num = checker_info_.horize_num;
  int32_t vert_num = checker_info_.vert_num;
  auto dw = checker_info_.checker_dw;
  auto dh = checker_info_.checker_dh;
  // 确定左上角点坐标，根据棋盘格大小确定别的角点坐标
  // 北西天坐标系，x为前进方向，y为前进方向左边
  double y0 = -((w - (horiz_num + 1) * dw) / 2.0 + (vert_num + 1) * dh + dw);
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

void CommonLayout::getLeftPt(std::vector<cv::Point3d> &left_world_pt) {
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

void CommonLayout::getRightPt(std::vector<cv::Point3d> &right_world_pt) {
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

void CommonLayout::getRearPt(std::vector<cv::Point3d> &rear_world_pt) {
  rear_world_pt.clear();
  auto w = (float)layout_param_.w;
  auto h = (float)layout_param_.h;
  int32_t horiz_num = checker_info_.horize_num;
  int32_t vert_num = checker_info_.vert_num;
  auto dw = (float)checker_info_.checker_dw;
  auto dh = (float)checker_info_.checker_dh;

  float x0 = -(h + (vert_num + 1) * dh + vert_num * dh);
  float y0 = -((w - (horiz_num + 1) * dw) / 2.0f + (vert_num + 1) * dh +
               horiz_num * dw);

  for (int32_t ivert = 0; ivert < vert_num; ++ivert) {
    for (int32_t ihoriz = 0; ihoriz < horiz_num; ++ihoriz) {
      float x = x0 + ivert * dh;
      float y = y0 + ihoriz * dw;
      cv::Point3d pt = cv::Point3d(x, y, 0.0);
      rear_world_pt.push_back(pt);
    }
  }
}

void CommonLayout::translate(const std::vector<cv::Point3d> &world_pts,
                             std::vector<cv::Point3d> &new_world_pts) {
  auto d = (float)layout_param_.d;
  auto w = (float)layout_param_.w;
  int32_t vert_num = checker_info_.vert_num;
  auto dh = (float)checker_info_.checker_dh;

  float car_length = car_info_.car_length();
  float car_width = car_info_.car_width();
  float car_front_wheelspan = car_info_.car_front_wheelspan();
  float car_rear_wheelspan = car_info_.car_rear_wheelspan();
  float car_wheelbase = car_info_.car_wheelbase();
  float car_wheelWidth = car_info_.car_wheelwidth();
  float car_front_overhang = car_info_.car_front_overhang();

  float y0 = w + (vert_num + 1) * 2 * dh;
  y0 /= 2.0f;
  float x0 = (car_front_overhang + car_wheelbase) + (vert_num + 1) * dh + d;

  new_world_pts.resize(world_pts.size());
  for (uint32_t inum = 0; inum < world_pts.size(); ++inum) {
    new_world_pts[inum].x = world_pts[inum].x + x0;
    new_world_pts[inum].y = world_pts[inum].y + y0;
    new_world_pts[inum].z = world_pts[inum].z;
  }
}

int CommonLayout::getCheckerBoardConnerSize(cv::Size &conner_size) {
  conner_size = cv::Size(checker_info_.horize_num, checker_info_.vert_num);
  return 0;
}
} // namespace avm
} // namespace minieye