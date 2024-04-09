//
// Created by minieye on 2022/6/1.
//

#include "aipilot/services/dfg_avm_mapping/camera/fisheye_model.h"

#include "aipilot/services/dfg_avm_mapping/camera/pinhole_model.h"
#include "glog/logging.h"

namespace minieye {
namespace base {
cv::Point2d FisheyeCameraDistortionModel::Project(const cv::Point3d &point3d) {
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
    const double k3 = distort_params_.val[2];
    const double k4 = distort_params_.val[3];

    // normalized
    const cv::Point2d pt_normalized(point3d.x / point3d.z,
                                    point3d.y / point3d.z);
    const double x_n = pt_normalized.x;
    const double y_n = pt_normalized.y;
    const double x_mul_x = x_n * x_n;
    const double y_mul_y = y_n * y_n;
    const double r = std::sqrt(x_mul_x + y_mul_y);
    const double theta = std::atan(r);
    const double theta_d =
        theta * (1 + k1 * pow(theta, 2) + k2 * pow(theta, 4) +
                 k3 * pow(theta, 6) + k4 * pow(theta, 8));
    // tangential distortion
    pt2d_img.x = theta_d / r * x_n;
    pt2d_img.y = theta_d / r * y_n;

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
FisheyeCameraDistortionModel::get_camera_model() {
  std::shared_ptr<PinholeCameraModel> camera_model(new PinholeCameraModel());
  camera_model->set_width(width_);
  camera_model->set_height(height_);
  camera_model->set_intrinsic_params(intrinsic_params_);

  return std::dynamic_pointer_cast<BaseCameraModel>(camera_model);
}

bool FisheyeCameraDistortionModel::set_params(size_t width, size_t height,
                                              const cv::Mat &intrins_params,
                                              const cv::Mat &distort_params) {
  bool ret = false;
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
  if (std::max(distort_params.rows, distort_params.cols) == 4) {
    memcpy(intrinsic_params_.val, intrins_.data, sizeof(double) * 9);
    memcpy(distort_params_.val, distort_.data, sizeof(double) * 4);
  }

  return ret;
}

cv::Point3d
FisheyeCameraDistortionModel::UnProject(const cv::Point2d &point2d) {
  // transform to image coordinates
  const double fx = intrinsic_params_(0, 0);
  const double fy = intrinsic_params_(1, 1);
  const double cx = intrinsic_params_(0, 2);
  const double cy = intrinsic_params_(1, 2);
  double x, y;
  x = (point2d.x - cx) / fx;
  y = (point2d.y - cy) / fy;

  double theta_d = sqrt(x * x + y * y);

  // radial distortion coefficients
  const double k1 = distort_params_.val[0];
  const double k2 = distort_params_.val[1];
  const double k3 = distort_params_.val[2];
  const double k4 = distort_params_.val[3];

  theta_d = std::min(std::max(static_cast<double>(-CV_PI / 2.), theta_d),
                     static_cast<double>(CV_PI / 2.));

  double theta = theta_d;
  double scale = 0.0;

  if (fabs(theta_d) > 1e-8) {
    // compensate distortion iteratively

    const double EPS = 1e-8; // or std::numeric_limits<float>::epsilon();

    for (int j = 0; j < 10; j++) {
      double theta2 = theta * theta, theta4 = theta2 * theta2,
             theta6 = theta4 * theta2, theta8 = theta6 * theta2;
      double k1_theta2 = k1 * theta2, k2_theta4 = k2 * theta4,
             k3_theta6 = k3 * theta6, k4_theta8 = k4 * theta8;
      double theta_fix =
          (theta * (1 + k1_theta2 + k2_theta4 + k3_theta6 + k4_theta8) -
           theta_d) /
          (1 + 3 * k1_theta2 + 5 * k2_theta4 + 7 * k3_theta6 + 9 * k4_theta8);
      theta = theta - theta_fix;
      if (fabs(theta_fix) < EPS) {
        break;
      }
    }
    scale = std::tan(theta) / theta_d;
  }
  x = x * scale;
  y = y * scale;

  return cv::Point3d(x, y, 1);
}

cv::Point2d
FisheyeCameraDistortionModel::distort(const cv::Point2d &undistort_pt) {
  const double fx = intrinsic_params_(0, 0);
  const double fy = intrinsic_params_(1, 1);
  const double cx = intrinsic_params_(0, 2);
  const double cy = intrinsic_params_(1, 2);

  // radial distortion coefficients
  const double k1 = distort_params_.val[0];
  const double k2 = distort_params_.val[1];
  const double k3 = distort_params_.val[2];
  const double k4 = distort_params_.val[3];

  double x, y;
  x = (undistort_pt.x - cx) / fx;
  y = (undistort_pt.y - cy) / fy;

  const double x_mul_x = x * x;
  const double y_mul_y = y * y;
  const double r = std::sqrt(x_mul_x + y_mul_y);
  const double theta = std::atan(r);
  const double theta_d = theta * (1 + k1 * pow(theta, 2) + k2 * pow(theta, 4) +
                                  k3 * pow(theta, 6) + k4 * pow(theta, 8));

  cv::Point2d distort_pt;
  // tangential distortion
  distort_pt.x = theta_d / r * x;
  distort_pt.y = theta_d / r * y;

  // transform to image coordinates
  distort_pt.x = fx * distort_pt.x + cx;
  distort_pt.y = fy * distort_pt.y + cy;

  return distort_pt;
}

cv::Point2d
FisheyeCameraDistortionModel::undistort(const cv::Point2d &distort_pt) {
  const double fx = intrinsic_params_(0, 0);
  const double fy = intrinsic_params_(1, 1);
  const double cx = intrinsic_params_(0, 2);
  const double cy = intrinsic_params_(1, 2);

  cv::Point3d unit_plane_pt = UnProject(distort_pt);
  // transform to image coordinates
  cv::Point2d undistort_pt;
  undistort_pt.x = fx * unit_plane_pt.x + cx;
  undistort_pt.y = fy * unit_plane_pt.y + cy;
  return undistort_pt;
}
} // namespace base
} // namespace minieye