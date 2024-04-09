//
// Created by minieye on 2022/6/1.
//

#ifndef AIPLORER_BROWN_MODEL_H
#define AIPLORER_BROWN_MODEL_H

#include "aipilot/services/dfg_avm_mapping/include/avm_camera/camera.h"

namespace minieye {
namespace base {
class BrownCameraDistortionModel : public BaseCameraDistortionModel {
public:
  BrownCameraDistortionModel() = default;

  ~BrownCameraDistortionModel() = default;

  cv::Point2d Project(const cv::Point3d &point3d) override;

  cv::Point3d UnProject(const cv::Point2d &point2d) override;

  cv::Point2d distort(const cv::Point2d &undistort_pt) override;

  cv::Point2d undistort(const cv::Point2d &distort_pt) override;

  std::shared_ptr<BaseCameraModel> get_camera_model() override;

  std::string name() const override { return "BrownCameraDistortionModel"; }

  bool set_params(size_t width, size_t height, const cv::Mat &intrins_params,
                  const cv::Mat &distort_params) override;

  inline cv::Matx33d get_intrinsic_params() const { return intrinsic_params_; }

  inline cv::Matx<double, 5, 1> get_distort_params() const {
    return distort_params_;
  }

protected:
  cv::Matx33d intrinsic_params_;
  cv::Matx<double, 5, 1> distort_params_;
};
} // namespace base
} // namespace minieye

#endif // AIPLORER_BROWN_MODEL_H
