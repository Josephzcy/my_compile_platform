//
// Created by minieye on 2022/6/23.
//

#ifndef AIPLORER_CAMERA_REMAPPING_H
#define AIPLORER_CAMERA_REMAPPING_H

#include <string>
#include <vector>

#include "aipilot/services/dfg_avm_mapping/include/avm_camera/camera.h"
#include "msg/precompiler/avm_calib_param.pb.h"
#include "opencv2/opencv.hpp"

using AvmCameraModelMap =
    std::map<uint32_t,
             std::shared_ptr<minieye::base::BaseCameraDistortionModel>>;

namespace minieye {
namespace avm {

typedef struct {
  float x;
  float y;
  float z;
  int camera_id1;
  float u1;
  float v1;
  float w1;
  int camera_id2;
  float u2;
  float v2;
  float w2;
} SrvLut;

class AvmRemapping {
public:
  bool initCamera(const std::vector<std::string> &cam_configs_paths);
  bool initCamera(const std::vector<std::string> &cam_configs_paths,
                  const std::string &car_info_str);
  bool initCamera(const std::vector<CameraParam> &camera_params,
                  const CarInfo &car_info);

  int32_t project(const cv::Point3d &pw, SrvLut &srv_lut);

  int32_t project(int32_t camera_id, const cv::Point3d &pw,
                  cv::Point2d &pix_pt);

  int32_t unproject(uint32_t camera_direction, const cv::Point2d &pix_pt,
                    cv::Point3d &world_pt);

  //---------------------------------------------------------------------------
  /**
   * 对图像点进行畸变矫正，1.0.3版本仅支持张正友鱼眼模型
   * @param camera_direction，相机的安装方向：
   *        FRONT = 1;REAR  = 2;LEFT  = 4;RIGHT = 8;
   * @param pt， 鱼眼图像上的点
   * @param undistort_pt，矫正点
   * @return
   */
  int32_t undistort(uint32_t camera_direction, const cv::Point2f &pt,
                    cv::Point2f &undistort_pt);

  /**
   * 给图像上的一点添加畸变，映射到鱼眼图像,1.0.3版本仅支持张正友鱼眼模型
   * @param camera_direction，相机的安装方向：
   *        FRONT = 1;REAR  = 2;LEFT  = 4;RIGHT = 8;
   * @param undistort_pt,无畸变点
   * @param distort_pt,鱼眼图像上的点
   * @return
   */
  int32_t distort(uint32_t camera_direction, const cv::Point2f &undistort_pt,
                  cv::Point2f &distort_pt);

  /**
   * 判断投影点是否在车内,是则返回true，否则false
   * @param world_pt
   * @return
   */
  bool isInCar(const cv::Point3f &world_pt);

  void getCarInfo(CarInfo &car_info) { car_info = car_info_; }

private:
  void getKRT();

  int32_t getCameraModel(
      const CameraParam &camera_param,
      std::shared_ptr<base::BaseCameraDistortionModel> &camera_model);

  /**
   * 按照逆时针设置0,1,2,3象限，北西天建立车身坐标系，判断点所在象限
   * @param world_pt
   * @return
   */
  int32_t getQuadrant(const cv::Point3f &world_pt);

  // 根据划分的象限,得到组成该象限画面所需要的图片通道号
  int32_t getQuadrantBlendImg(int32_t quard_num, int32_t &left_ch,
                              int32_t &right_ch);

  /**
   * 根据世界坐标点所在象限，确定左边相机的融合权重
   * @param quard_num
   * @param angle_offset
   * @param world_pt
   * @param weight
   * @return
   */
  int32_t cal_blend(int32_t quard_num, int32_t angle_offset,
                    const cv::Point3d &world_pt, float &weight);

private:
  CarInfo car_info_;
  cv::Size img_size_;
  std::vector<cv::Mat> Hs_;
  std::vector<cv::Mat> Ks_;
  std::vector<cv::Mat> RTs_;
  std::map<uint32_t, CameraParam> camera_params_;
  AvmCameraModelMap camera_models_;
};

} // namespace avm
} // namespace minieye
#endif // AIPLORER_CAMERA_REMAPPING_H
