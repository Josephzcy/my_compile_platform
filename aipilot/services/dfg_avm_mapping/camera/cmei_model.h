//
// Created by minieye on 2022/6/1.
//

#ifndef AIPLORER_CMEI_MODEL_H
#define AIPLORER_CMEI_MODEL_H

#include "aipilot/services/dfg_avm_mapping/include/avm_camera/camera.h"
#include "opencv2/opencv.hpp"

namespace minieye {
namespace base {
class CMeiCameraDistortionModel : public BaseCameraDistortionModel {
public:
  CMeiCameraDistortionModel() = default;

  ~CMeiCameraDistortionModel() = default;

  cv::Point2d Project(const cv::Point3d &point3d) override;

  cv::Point3d UnProject(const cv::Point2d &point2d) override;

  cv::Point2d distort(const cv::Point2d &undistort_pt) override;

  cv::Point2d undistort(const cv::Point2d &distort_pt) override;

  std::shared_ptr<BaseCameraModel> get_camera_model() override;

  std::string name() const override { return "CMeiCameraDistortionModel"; }

  bool set_params(size_t width, size_t height, const cv::Mat &intrins_params,
                  const cv::Mat &distort_params) override;

  inline cv::Matx33d get_intrinsic_params() const { return intrinsic_params_; }

  // k1,k2,p1,p2,xi
  inline const cv::Matx<double, 1, 5> get_distort_params() const {
    return distort_params_;
  }

protected:
  cv::Matx33d intrinsic_params_;
  cv::Matx<double, 1, 5> distort_params_;
};
} // namespace base
} // namespace minieye

#endif // AIPLORER_CMEI_MODEL_H
