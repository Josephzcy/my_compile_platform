//
// Created by minieye on 2022/6/1.
//

#include "aipilot/services/dfg_avm_mapping/camera/pinhole_model.h"

namespace minieye {
namespace base {
cv::Point2d PinholeCameraModel::Project(const cv::Point3d &point3d) {
  cv::Point2d pt2d;
  double fx = intrinsic_params_(0, 0);
  double fy = intrinsic_params_(1, 1);
  double s = intrinsic_params_(0, 1);
  double cx = intrinsic_params_(0, 2);
  double cy = intrinsic_params_(1, 2);

  pt2d.x = point3d.x / point3d.z;
  pt2d.y = point3d.y / point3d.z;

  pt2d.x = pt2d.x * fx + s * pt2d.y + cx;
  pt2d.y = pt2d.y * fy + cy;

  return pt2d;
}

cv::Point3d PinholeCameraModel::UnProject(const cv::Point2d &point2d) {
  cv::Point3d pt3d;
  pt3d.x = (point2d.x - intrinsic_params_(0, 2)) / intrinsic_params_(0, 0);
  pt3d.y = (point2d.y - intrinsic_params_(1, 2)) / intrinsic_params_(1, 1);
  pt3d.z = 1.0f;

  return pt3d;
}
} // namespace base
} // namespace minieye