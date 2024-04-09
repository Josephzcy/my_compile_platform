// Copyright [2022] - MiniEye INC.
#ifndef AVM_MAPPING_H
#define AVM_MAPPING_H

#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

struct AvmMapping {
  std::vector<int> camera_ids;        // 参与映射的摄像头列表
  std::vector<cv::Point2f> image_pts; // 每个摄像头成像中对应的像素坐标
  std::vector<float> weights; // 每路图像像素坐标的权重,以上三者一一对应
  float brightness_coeff; // 亮度调整系数

  AvmMapping() {
    camera_ids.clear();
    image_pts.clear();
    weights.clear();
    brightness_coeff = 1;
  }
};

class AVMMapper {
public:
  AVMMapper();
  ~AVMMapper();
  // --------------------------------------------------------------------------
  /// 加载相机内参进行畸变矫正
  ///  @brief InitCameraParams 初始化各路摄像头参数
  ///
  ///  @param cam_configs_paths : 摄像头参数配置文件路径列表
  ///  @retval : true - 成功，false - 失败
  // --------------------------------------------------------------------------
  bool InitCameraParams(const std::vector<std::string> &cam_configs_paths);

  /**
   * 加载内外参以及车身信息，计算像素坐标与车身坐标系的转换以及计算融合权重
   * @param cam_configs_paths，相机内外参文件列表
   * @param car_info，车身信息文件
   * @return
   */
  bool InitCameraParams(const std::vector<std::string> &cam_configs_paths,
                        const std::string &car_info);

  // --------------------------------------------------------------------------
  /// @brief GetMapping 获取某一世界坐标的avm映射配置
  ///
  /// @param world_pt : 车身坐标系下地面上某一位置的坐标
  /// @retval : 对应avm映射配置
  // --------------------------------------------------------------------------
  AvmMapping GetMapping(const cv::Point3f &world_pt);

  //---------------------------------------------------------------------------
  /**
   * 对图像点进行畸变矫正
   * @param camera_direction，相机的安装方向：
   *        FRONT = 1;REAR  = 2;LEFT  = 4;RIGHT = 8;
   * @param pt， 鱼眼图像上的点
   * @param undistort_pt，矫正点
   * @return
   */
  int32_t UnDistort(uint32_t camera_direction, const cv::Point2f &pt,
                    cv::Point2f &undistort_pt);

  /**
   * 给图像上的一点添加畸变，映射到鱼眼图像
   * @param camera_direction，相机的安装方向：
   *        FRONT = 1;REAR  = 2;LEFT  = 4;RIGHT = 8;
   * @param undistort_pt,无畸变点
   * @param distort_pt,鱼眼图像上的点
   * @return
   */
  int32_t Distort(uint32_t camera_direction, const cv::Point2f &undistort_pt,
                  cv::Point2f &distort_pt);

  int32_t pix2world(uint32_t camera_direction, const cv::Point2f &pix_pt,
                    cv::Point3f &world_pt);

  int32_t world2pix(uint32_t camera_direction, const cv::Point3f &world_pt,
                    cv::Point2f &pix_pt);

private:
  /**
   * 每个像素最多只有两个图像的纹理，但是现在纹理坐标是基于整个区域进行渲染的，需要顺序添加所有纹理
   * @param avm_mapper_info，两个纹理的融合信息
   * @param newMapper，包含所有纹理的融合信息
   * @return
   */
  int32_t appendTerxture(const AvmMapping &avm_mapper_info,
                         AvmMapping &newMapper);

private:
  void *camera_remapping_;
};

#endif //AVM_MAPPING_H