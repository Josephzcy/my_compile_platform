//
// Created by minieye on 2022/6/1.
//

#ifndef AIPLORER_PINHOLECAMERAMODEL_H
#define AIPLORER_PINHOLECAMERAMODEL_H

#include "aipilot/services/dfg_avm_mapping/include/avm_camera/camera.h"
#include "opencv2/opencv.hpp"

namespace minieye {
namespace base {
class PinholeCameraModel : public BaseCameraModel {
public:
  ~PinholeCameraModel() = default;

  cv::Point2d Project(const cv::Point3d &point3f) override;
  cv::Point3d UnProject(const cv::Point2d &point2f) override;
  std::string name() const override { return "PinholeCameraModel"; }

  inline void set_intrinsic_params(const cv::Matx33d &params) {
    intrinsic_params_ = params;
  }

  inline cv::Matx33d get_intrinsic_params() const { return intrinsic_params_; }

protected:
  /*     fx  0   cx
         0   fy  cy
         0    0  1
  */
  cv::Matx33d intrinsic_params_;
};
} // namespace base
} // namespace minieye

#endif // AIPLORER_PINHOLECAMERAMODEL_H
