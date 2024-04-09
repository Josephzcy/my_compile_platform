//
// Created by minieye on 2022/6/15.
//

#ifndef AIPLORER_DAVIDE_SCARAMUZZA_MODEL_H
#define AIPLORER_DAVIDE_SCARAMUZZA_MODEL_H

#include "aipilot/services/dfg_avm_mapping/include/avm_camera/camera.h"
#include "opencv2/opencv.hpp"

#define MAX_POL_LENGTH (64)
#define SF (4)

namespace minieye {
namespace base {

class DavideScaramCameraDistortionModel : public BaseCameraDistortionModel {
public:
  DavideScaramCameraDistortionModel() = default;

  ~DavideScaramCameraDistortionModel() = default;
  std::shared_ptr<BaseCameraModel> get_camera_model() override;
  cv::Point2d Project(const cv::Point3d &point3d) override;

  cv::Point3d UnProject(const cv::Point2d &point2d) override;

  cv::Point2d distort(const cv::Point2d &undistort_pt) override;

  cv::Point2d undistort(const cv::Point2d &distort_pt) override;

  std::string name() const override {
    return "DavideScaramCameraDistortionModel";
  }

  /**
   * distort_params:
   *  - ploy_param
   *  - affine_param
   *  - center
   *  - invploy_param
   */

  bool set_params(size_t width, size_t height, const cv::Mat &intrinsic_params,
                  const cv::Mat &distort_params) override;

protected:
  // ocam model
  int32_t invploy_length_;
  double ploy_param_[5];
  double center_[2];
  double invploy_param_[MAX_POL_LENGTH];
  cv::Matx<double, 2, 2> affine_param_;
  cv::Matx33d intrinsic_params_;
};
} // namespace base
} // namespace minieye

#endif // AIPLORER_DAVIDE_SCARAMUZZA_MODEL_H
