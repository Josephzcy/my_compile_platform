//
// Created by minieye on 2022/6/21.
//

#include "aipilot/services/dfg_avm_mapping/src/avm_parse.h"

#include <fstream>

#include "glog/logging.h"
#include "json.hpp"
#include "opencv2/opencv.hpp"

namespace minieye {
namespace avm {
int32_t writeJson(const CameraParam &cameraParams,
                  const std::string &extrins_json) {
  LOG(INFO) << "success to calib extrins, write:" << extrins_json;

  std::vector<std::string> blend_name = {};
  nlohmann::json root;
  root["camera_id"] = cameraParams.camera_id();
  root["install_direction"] = cameraParams.camera_id();
  root["prj_model"] = cameraParams.prj_model();
  root["image_width"] = cameraParams.image_width();
  root["image_height"] = cameraParams.image_height();
  root["focal_u"] = cameraParams.focal_u();
  root["focal_v"] = cameraParams.focal_v();
  root["cu"] = cameraParams.cu();
  root["cv"] = cameraParams.cv();

  uint32_t start_inum = 0;
  if (2 == cameraParams.prj_model()) {
    root["s"] = cameraParams.distort_coeffs(0);
    start_inum = 1;
  }
  for (uint32_t inum = start_inum;
       inum < (uint32_t)cameraParams.distort_coeffs_size(); ++inum) {
    root["distort_coeffs"].push_back(cameraParams.distort_coeffs(inum));
  }

  // homo
  auto trans_mtx = cameraParams.trans_mtx();
  root["vcsgnd2img"] = {trans_mtx.vcsgnd2img(0), trans_mtx.vcsgnd2img(1),
                        trans_mtx.vcsgnd2img(2), trans_mtx.vcsgnd2img(3),
                        trans_mtx.vcsgnd2img(4), trans_mtx.vcsgnd2img(5),
                        trans_mtx.vcsgnd2img(6), trans_mtx.vcsgnd2img(7),
                        trans_mtx.vcsgnd2img(8)};
  root["pitch"] = cameraParams.pitch();
  root["yaw"] = cameraParams.yaw();
  root["roll"] = cameraParams.roll();

  root["pos"] = {cameraParams.pos(0), cameraParams.pos(1), cameraParams.pos(2)};

  // 输出到文件
  std::ofstream os;
  os.open(extrins_json, std::ios::out);
  os << root.dump(4) << std::endl;
  os.close();
  return 0;
}

bool parseIntrinsJson(const std::string &json_file,
                      minieye::CameraParam *camera_param) {
  // load 内参
  bool ret = true;
  nlohmann::json config_json;
  std::ifstream fin(json_file);
  if (!fin.is_open()) {
    LOG(ERROR) << "failed to load intrins file : " << json_file;
    ret = false;
  } else {
    fin >> config_json;
    camera_param->set_prj_model(config_json.at("prj_model"));
    camera_param->set_camera_id(config_json.at("camera_id"));
    camera_param->set_image_width(config_json.at("image_width"));
    camera_param->set_image_height(config_json.at("image_height"));
    camera_param->set_focal_u(config_json.at("focal_u"));
    camera_param->set_focal_v(config_json.at("focal_v"));

    if (config_json.find("install_direction") != config_json.end()) {
      camera_param->set_install_direction(config_json.at("install_direction"));
    }

    camera_param->set_camera_id(config_json.at("camera_id"));
    camera_param->set_cu(config_json.at("cu"));
    camera_param->set_cv(config_json.at("cv"));
    uint32_t ploy_length = config_json["distort_coeffs"].size();

    if (config_json.find("pitch") != config_json.end()) {
      camera_param->set_pitch(config_json["pitch"]);
    }
    if (config_json.find("yaw") != config_json.end()) {
      camera_param->set_yaw(config_json["yaw"]);
    }
    if (config_json.find("roll") != config_json.end()) {
      camera_param->set_roll(config_json["roll"]);
    }
    if (config_json.find("pos") != config_json.end()) {
      for (uint32_t i = 0; i < 3; ++i) {
        camera_param->add_pos(config_json["pos"][i]);
      }
    }
    if (2 == config_json.at("prj_model")) {
      camera_param->add_distort_coeffs(config_json["s"]);
    }
    minieye::TransMatrix *ptr = camera_param->mutable_trans_mtx();
    if (config_json.find("vcsgnd2img") != config_json.end()) {
      for (uint32_t inum = 0; inum < 9; ++inum) {
        ptr->add_vcsgnd2img(config_json["vcsgnd2img"][inum]);
      }
    }
    for (uint32_t i = 0; i < ploy_length; ++i) {
      camera_param->add_distort_coeffs(config_json["distort_coeffs"][i]);
    }
  }
  fin.close();
  return ret;
}

bool parseExtrinsJson(const std::string &extrins_file, CarInfo *car_info,
                      AvmExtrinsInfo *extrins_info,
                      AvmCalibMethod &calib_method) {
  bool ret = true;
  // load 内参
  nlohmann::json config_json;
  std::ifstream fin(extrins_file);
  if (!fin.is_open()) {
    LOG(ERROR) << "failed to load extrins file : " << extrins_file;
    ret = false;
  } else {
    fin >> config_json;
    calib_method = config_json.at("calib_method");

    car_info->set_car_length(config_json.at("car_length"));
    car_info->set_car_width(config_json.at("car_width"));
    car_info->set_car_front_wheelspan(config_json.at("car_front_wheelspan"));
    car_info->set_car_rear_wheelspan(config_json.at("car_rear_wheelspan"));
    car_info->set_car_wheelbase(config_json.at("car_wheelbase"));
    car_info->set_car_wheelwidth(config_json.at("car_wheelwidth"));
    car_info->set_car_front_overhang(config_json.at("car_front_overhang"));
    car_info->set_car_front_blend(config_json.at("car_front_blend"));
    car_info->set_car_rear_blend(config_json.at("car_rear_blend"));
    car_info->set_car_left_blend(config_json.at("car_left_blend"));
    car_info->set_car_right_blend(config_json.at("car_right_blend"));

    extrins_info->set_checker_dw(config_json.at("checker_dw"));
    extrins_info->set_checker_dh(config_json.at("checker_dh"));
    extrins_info->set_vert_num(config_json.at("vert_num"));
    extrins_info->set_horize_num(config_json.at("horize_num"));
    extrins_info->set_w(config_json.at("w"));
    extrins_info->set_h(config_json.at("h"));
    extrins_info->set_d(config_json.at("d"));
    extrins_info->set_v(config_json.at("v"));
  }

  fin.close();
  return ret;
}

bool parseCarInfoJson(const std::string &carInfo_file, CarInfo *car_info) {
  bool ret = true;
  // load 内参
  nlohmann::json config_json;
  std::ifstream fin(carInfo_file);
  if (!fin.is_open()) {
    LOG(ERROR) << "failed to load carInfo file : " << carInfo_file;
    ret = false;
  } else {
    fin >> config_json;

    car_info->set_car_length(config_json.at("car_length"));
    car_info->set_car_width(config_json.at("car_width"));
    car_info->set_car_front_wheelspan(config_json.at("car_front_wheelspan"));
    car_info->set_car_rear_wheelspan(config_json.at("car_rear_wheelspan"));
    car_info->set_car_wheelbase(config_json.at("car_wheelbase"));
    car_info->set_car_wheelwidth(config_json.at("car_wheelwidth"));
    car_info->set_car_front_overhang(config_json.at("car_front_overhang"));
    car_info->set_car_front_blend(config_json.at("car_front_blend"));
    car_info->set_car_rear_blend(config_json.at("car_rear_blend"));
    car_info->set_car_left_blend(config_json.at("car_left_blend"));
    car_info->set_car_right_blend(config_json.at("car_right_blend"));
  }

  fin.close();
  return ret;
}

bool parseIntrinsExtrinsJson(const std::string &json_file,
                             minieye::CameraParam *camera_param,
                             minieye::avm::CarInfo *car_info) {
  // load 内参
  bool ret = true;
  nlohmann::json config_json;
  std::ifstream fin(json_file);
  if (!fin.is_open()) {
    LOG(ERROR) << "failed to load intrins file : " << json_file;
    ret = false;
  } else {
    fin >> config_json;
    camera_param->set_prj_model(config_json.at("prj_model"));
    camera_param->set_camera_id(config_json.at("camera_id"));
    camera_param->set_image_width(config_json.at("image_width"));
    camera_param->set_image_height(config_json.at("image_height"));
    camera_param->set_focal_u(config_json.at("focal_u"));
    camera_param->set_focal_v(config_json.at("focal_v"));
    camera_param->set_install_direction(config_json.at("install_direction"));
    camera_param->set_camera_id(config_json.at("camera_id"));
    camera_param->set_cu(config_json.at("cu"));
    camera_param->set_pitch(config_json["pitch"]);
    camera_param->set_yaw(config_json["yaw"]);
    camera_param->set_roll(config_json["roll"]);
    camera_param->set_cv(config_json.at("cv"));
    uint32_t ploy_length = config_json["distort_coeffs"].size();

    for (uint32_t i = 0; i < ploy_length; ++i) {
      camera_param->add_distort_coeffs(config_json["distort_coeffs"][i]);
    }

    for (uint32_t i = 0; i < 3; ++i) {
      camera_param->add_pos(config_json["pos"][i]);
    }

    minieye::TransMatrix *ptr = camera_param->mutable_trans_mtx();
    for (uint32_t inum = 0; inum < 9; ++inum) {
      ptr->add_vcsgnd2img(config_json["vcsgnd2img"][inum]);
    }

    car_info->set_car_length(config_json["car_length"]);
    car_info->set_car_front_overhang(config_json["car_front_overhang"]);
    car_info->set_car_wheelbase(config_json["car_wheelbase"]);
    car_info->set_car_width(config_json["car_width"]);
    car_info->set_car_front_blend(config_json.at("car_front_blend"));
    car_info->set_car_rear_blend(config_json.at("car_rear_blend"));
    car_info->set_car_left_blend(config_json.at("car_left_blend"));
    car_info->set_car_right_blend(config_json.at("car_right_blend"));
  }
  fin.close();
  return ret;
}

} // namespace avm
} // namespace minieye