//
// Created by minieye on 2022/5/12.
//
#include <fstream>

#include "aipilot/services/dfg_avm_calib/src/avm_calib.h"
#include "aipilot/services/dfg_avm_calib/src/extrins_calib.h"
#include "glog/logging.h"
#include "msg/precompiler/avm_calib_param.pb.h"

using namespace minieye::avm;

AvmHandle initAvmCalib(const char camera_file[AVM_CAMERA_NUM][128],
                       const char layout_file[128],
                       const char car_info_file[128]) {
  AvmErrorCode ret_code;
  AvmHandle handle = nullptr;

  std::vector<std::string> intrins_file;
  for (uint32_t inum = 0; inum < 4; ++inum) {
    std::string file = std::string(camera_file[inum]);
    intrins_file.emplace_back(file);
  }
  std::string car_info_str = std::string(car_info_file);
  std::string layout_str = std::string(layout_file);

  auto *extrins_calib = new AvmExtrinsCalib;
  ret_code = extrins_calib->init(intrins_file, layout_file, car_info_file);

  if (AVM_NORMAL_STATE == ret_code) {
    handle = (AvmHandle)extrins_calib;
  } else {
    LOG(WARNING) << "abnormal exit, code:" << ret_code;
    delete extrins_calib;
  }
  return handle;
}

void avmRelease(AvmHandle handle) {
  if (nullptr != handle) {
    auto *calib_info = (AvmExtrinsCalib *)handle;
    delete calib_info;
  }
}

float avmCalib(AvmHandle handle, unsigned int camera_id,
               const unsigned char *image_data, const char *calib_file) {
  float residual = -1.0f;
  AvmErrorCode ret = AVM_NORMAL_STATE;
  if (nullptr != handle) {
    auto *calib_info = (AvmExtrinsCalib *)handle;
    ret = calib_info->calib((minieye::CamDirect)camera_id, image_data,
                            std::string(calib_file), residual);
    if (AVM_NORMAL_STATE != ret) {
      LOG(ERROR) << "failed to calib, camera_id:" << camera_id
                 << ",\terrcode: " << ret;
      residual = -(float)ret;
    }
  } else {
    LOG(ERROR) << "empty handle";
  }

  return residual;
}