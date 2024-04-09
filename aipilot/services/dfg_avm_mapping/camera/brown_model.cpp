//
// Created by minieye on 2022/6/1.
//

#include "aipilot/services/dfg_avm_mapping/camera/brown_model.h"

#include "aipilot/services/dfg_avm_mapping/camera/pinhole_model.h"
#include "glog/logging.h"

namespace minieye {
namespace base {
cv::Point2d BrownCameraDistortionModel::Project(const cv::Point3d &point3d) {
  cv::Point2d pt2d_img;
  if (std::isless(point3d.z, 0.f)) {
    LOG(ERROR) << "The input point (" << point3d
               << ") should be in front of the camera";
    pt2d_img.x = DBL_MAX;
    pt2d_img.y = DBL_MAX;
  } else {
    // radial distortion coefficients
    const double k1 = distort_params_.val[0];
    const double k2 = distort_params_.val[1];
    const double k3 = distort_params_.val[4];
    // tangential distortion coefficients
    const double p1 = distort_params_.val[2];
    const double p2 = distort_params_.val[3];

    // normalized
    const cv::Point2d pt_normalized(point3d.x / point3d.z,
                                    point3d.y / point3d.z);
    const double x_n = pt_normalized.x;
    const double y_n = pt_normalized.y;
    const double x_mul_x = x_n * x_n;
    const double y_mul_y = y_n * y_n;
    const double x_mul_y = x_n * y_n;
    const double r_squared = x_mul_x + y_mul_y;
    const double r_to_the_4th = r_squared * r_squared;
    const double r_to_the_6th = r_squared * r_to_the_4th;

    // radial distortion
    pt2d_img = pt_normalized *
               (1 + k1 * r_squared + k2 * r_to_the_4th + k3 * r_to_the_6th);

    // tangential distortion
    pt2d_img.x += 2 * p1 * x_mul_y + p2 * (r_squared + 2 * x_mul_x);
    pt2d_img.y += p1 * (r_squared + 2 * y_mul_y) + 2 * p2 * x_mul_y;

    // transform to image coordinates
    const double fx = intrinsic_params_(0, 0);
    const double fy = intrinsic_params_(1, 1);
    const double cx = intrinsic_params_(0, 2);
    const double cy = intrinsic_params_(1, 2);
    pt2d_img.x = fx * pt2d_img.x + cx;
    pt2d_img.y = fy * pt2d_img.y + cy;
  }
  return pt2d_img;
}

std::shared_ptr<BaseCameraModel>
BrownCameraDistortionModel::get_camera_model() {
  std::shared_ptr<PinholeCameraModel> camera_model(new PinholeCameraModel());
  camera_model->set_width(width_);
  camera_model->set_height(height_);
  camera_model->set_intrinsic_params(intrinsic_params_);

  return std::dynamic_pointer_cast<BaseCameraModel>(camera_model);
}

bool BrownCameraDistortionModel::set_params(size_t width, size_t height,
                                            const cv::Mat &intrins_params,
                                            const cv::Mat &distort_params) {
  width_ = width;
  height_ = height;

  cv::Mat intrins_ = intrins_params;
  cv::Mat distort_ = distort_params;

  if (intrins_.depth() != CV_64F) {
    intrins_.convertTo(intrins_, CV_64F);
  }
  if (distort_.depth() != CV_64F) {
    distort_.convertTo(intrins_, CV_64F);
  }
  memcpy(intrinsic_params_.val, intrins_.data, sizeof(double) * 9);
  memcpy(distort_params_.val, distort_.data, sizeof(double) * 5);
  return true;
}

cv::Point3d BrownCameraDistortionModel::UnProject(const cv::Point2d &point2d) {
  return cv::Point3d();
}

cv::Point2d
BrownCameraDistortionModel::distort(const cv::Point2d &undistort_pt) {
  return cv::Point2d();
}

cv::Point2d
BrownCameraDistortionModel::undistort(const cv::Point2d &distort_pt) {
  return cv::Point2d();
}
} // namespace base
} // namespace minieye