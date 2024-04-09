//
// Created by minieye on 2022/4/24.
//

#include "aipilot/services/dfg_avm_mapping/include/avm_mapping/avm_mapping.h"

#include <fstream>

#include "aipilot/services/dfg_avm_mapping/src/avm_parse.h"
#include "aipilot/services/dfg_avm_mapping/src/camera_remapping.h"
#include "glog/logging.h"
#include "json.hpp"
#include "msg/precompiler/avm_calib_param.pb.h"
#include "msg/precompiler/camera.pb.h"

using namespace minieye::avm;
using namespace minieye;

bool AVMMapper::InitCameraParams(
    const std::vector<std::string> &cam_configs_paths,
    const std::string &car_info_str) {
  bool ret = true;
  auto *ptr = (minieye::avm::AvmRemapping *)camera_remapping_;

  std::vector<CameraParam> camera_params;
  camera_params.resize(cam_configs_paths.size());
  for (uint32_t inum = 0; inum < cam_configs_paths.size(); ++inum) {
    CameraParam camera_param;
    ret = parseIntrinsJson(cam_configs_paths[inum], &camera_param);

    uint32_t camera_id = camera_param.camera_id();
    uint32_t ch = (uint32_t)std::log2(camera_id);
    camera_params[ch] = camera_param;
  }
  if (camera_params.size() != 4) {
    LOG(ERROR) << "fail to load intrins";
    ret = false;
  }
  CarInfo car_info;
  if (ret) {
    ret = parseCarInfoJson(car_info_str, &car_info);
  }

  ret = ptr->initCamera(camera_params, car_info);
  return ret;
}

bool AVMMapper::InitCameraParams(
    const std::vector<std::string> &cam_configs_paths) {
  auto *ptr = (minieye::avm::AvmRemapping *)camera_remapping_;
  return ptr->initCamera(cam_configs_paths);
}

int32_t AVMMapper::appendTerxture(const AvmMapping &avm_mapper_info,
                                  AvmMapping &newMapper) {
  int32_t ret = 0;
  newMapper.weights.clear();
  newMapper.image_pts.clear();
  newMapper.camera_ids.clear();
  for (uint32_t i = 0; i < 4; ++i) {
    newMapper.camera_ids.push_back(std::pow(2, i));
    newMapper.image_pts.emplace_back(0, 0);
    newMapper.weights.push_back(0);
    newMapper.brightness_coeff = 1.0f;
  }
  for (uint32_t i = 0; i < 2; ++i) {
    int32_t ch = avm_mapper_info.camera_ids[i];
    ch = std::log2(ch);
    newMapper.image_pts[ch] = avm_mapper_info.image_pts[i];
    newMapper.weights[ch] += avm_mapper_info.weights[i];
  }
  return ret;
}

AvmMapping AVMMapper::GetMapping(const cv::Point3f &world_pt) {
  AvmMapping mapping_info, ret_mapping;
  mapping_info.camera_ids.resize(2);
  mapping_info.image_pts.resize(2);
  mapping_info.weights.resize(2);
  mapping_info.brightness_coeff = 1.0f;

  auto *ptr = (minieye::avm::AvmRemapping *)camera_remapping_;
  cv::Point3d pw(world_pt.x, world_pt.y, world_pt.z);
  minieye::avm::SrvLut srv_lut;
  ptr->project(pw, srv_lut);

  if (ptr->isInCar(pw)) {
    srv_lut.w1 = 0.0f;
    srv_lut.w2 = 0.0f;
  }

  mapping_info.camera_ids[0] = srv_lut.camera_id1;
  mapping_info.camera_ids[1] = srv_lut.camera_id2;
  mapping_info.image_pts[0] = cv::Point2f(srv_lut.u1, srv_lut.v1);
  mapping_info.image_pts[1] = cv::Point2f(srv_lut.u2, srv_lut.v2);
  mapping_info.weights[0] = srv_lut.w1;
  mapping_info.weights[1] = srv_lut.w2;

  appendTerxture(mapping_info, ret_mapping);
  return ret_mapping;
}

int32_t AVMMapper::UnDistort(uint32_t camera_direction, const cv::Point2f &pt,
                             cv::Point2f &undistort_pt) {
  int32_t ret = 0;
  auto *ptr = (minieye::avm::AvmRemapping *)camera_remapping_;
  ptr->undistort(camera_direction, pt, undistort_pt);
  return ret;
}

int32_t AVMMapper::Distort(uint32_t camera_direction,
                           const cv::Point2f &undistort_pt,
                           cv::Point2f &distort_pt) {
  int32_t ret = 0;
  auto *ptr = (minieye::avm::AvmRemapping *)camera_remapping_;
  ptr->distort(camera_direction, undistort_pt, distort_pt);
  return ret;
}

int32_t AVMMapper::pix2world(uint32_t camera_direction,
                             const cv::Point2f &pix_pt, cv::Point3f &world_pt) {
  int32_t ret = 0;
  auto *ptr = (minieye::avm::AvmRemapping *)camera_remapping_;
  cv::Point3d ret_world_pt;
  ptr->unproject(camera_direction, pix_pt, ret_world_pt);
  world_pt.x = ret_world_pt.x;
  world_pt.y = ret_world_pt.y;
  return ret;
}

AVMMapper::AVMMapper() { camera_remapping_ = new minieye::avm::AvmRemapping; }

AVMMapper::~AVMMapper() {
  if (camera_remapping_ != nullptr) {
    auto *ptr = (minieye::avm::AvmRemapping *)camera_remapping_;
    delete ptr;
  }
}

int32_t AVMMapper::world2pix(uint32_t camera_direction,
                             const cv::Point3f &world_pt, cv::Point2f &pix_pt) {
  auto *ptr = (minieye::avm::AvmRemapping *)camera_remapping_;
  cv::Point3d pw(world_pt.x, world_pt.y, world_pt.z);
  minieye::avm::SrvLut srv_lut;
  ptr->project(pw, srv_lut);
  if ((int)camera_direction == srv_lut.camera_id1) {
    pix_pt = cv::Point2f(srv_lut.u1, srv_lut.v1);
  } else {
    pix_pt = cv::Point2f(srv_lut.u2, srv_lut.v2);
  }
  return 0;
}