//
// Created by minieye on 2022/6/21.
//

#ifndef AIPLORER_AVM_PARSE_H
#define AIPLORER_AVM_PARSE_H
#include "msg/precompiler/avm_calib_param.pb.h"

namespace minieye {
namespace avm {
/**
 * 将相机参数存于json文件
 * @param cameraParams，相机参数
 * @param extrins_json，json名字
 * @return
 */
int32_t writeJson(const CameraParam &cameraParams,
                  const std::string &extrins_json);
/**
 * DEPRECATED interface
 * @param json_file
 * @param camera_param
 * @param car_info
 * @return
 */
bool parseIntrinsExtrinsJson(const std::string &json_file,
                             minieye::CameraParam *camera_param,
                             minieye::avm::CarInfo *car_info);
/**
 * 解析相机内参及畸变系数
 * @param json_file
 * @param camera_param
 * @return
 */
bool parseIntrinsJson(const std::string &json_file,
                      minieye::CameraParam *camera_param);

bool parseExtrinsJson(const std::string &extrins_file, CarInfo *car_info,
                      AvmExtrinsInfo *extrins_info,
                      AvmCalibMethod &calib_method);

/**
 * 解析车身信息
 * @param carInfo_file
 * @param car_info
 * @return
 */
bool parseCarInfoJson(const std::string &carInfo_file, CarInfo *car_info);
} // namespace avm
} // namespace minieye
#endif // AIPLORER_AVM_PARSE_H
