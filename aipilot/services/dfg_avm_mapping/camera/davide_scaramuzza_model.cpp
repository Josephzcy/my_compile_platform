//
// Created by minieye on 2022/6/15.
//

#include "aipilot/services/dfg_avm_mapping/camera/davide_scaramuzza_model.h"

#include "aipilot/services/dfg_avm_mapping/camera/pinhole_model.h"
#include "glog/logging.h"

namespace minieye {
namespace base {
std::shared_ptr<BaseCameraModel>
DavideScaramCameraDistortionModel::get_camera_model() {
  std::shared_ptr<PinholeCameraModel> camera_model(new PinholeCameraModel());
  camera_model->set_width(width_);
  camera_model->set_height(height_);
  camera_model->set_intrinsic_params(intrinsic_params_);

  return std::dynamic_pointer_cast<BaseCameraModel>(camera_model);
}

bool DavideScaramCameraDistortionModel::set_params(
    size_t width, size_t height, const cv::Mat &intrinsic_params,
    const cv::Mat &distort_params) {
  width_ = width;
  height_ = height;
  cv::Mat in_intrins, in_distort;
  intrinsic_params.convertTo(in_intrins, CV_64F);
  intrinsic_params_ = in_intrins;
  distort_params.convertTo(in_distort, CV_64F);
  uint32_t offset = 0;
  center_[0] = in_intrins.at<double>(0, 2);
  center_[1] = in_intrins.at<double>(1, 2);

  memcpy(ploy_param_, in_distort.data, sizeof(double) * 5);
  offset += sizeof(double) * 5;
  memcpy(affine_param_.val, in_distort.data + offset, sizeof(double) * 4);
  offset += sizeof(double) * 4;
  invploy_length_ = std::max(in_distort.cols, in_distort.rows) - 9;

  bool ret = true;
  if (invploy_length_ > 0) {
    memcpy(invploy_param_, distort_params.data + offset,
           sizeof(double) * invploy_length_);
  } else {
    ret = false;
  }
  return ret;
}

cv::Point2d
DavideScaramCameraDistortionModel::Project(const cv::Point3d &point3d) {
  cv::Point2d pt2d_img;
  double *invpol = invploy_param_;
  double c = affine_param_.val[0];
  double d = affine_param_.val[1];
  double e = affine_param_.val[2];
  double xc = center_[1];
  double yc = center_[0];
  double norm = sqrt(point3d.x * point3d.x + point3d.y * point3d.y);
  double theta = atan(point3d.z / norm);
  double t, t_i;
  double rho, x, y;
  double invnorm;
  int i;

  if (norm != 0) {
    invnorm = 1 / norm;
    t = theta;
    rho = invpol[0];
    t_i = 1;

    for (i = 1; i < invploy_length_; i++) {
      t_i *= t;
      rho += t_i * invpol[i];
    }

    x = point3d.x * invnorm * rho;
    y = point3d.y * invnorm * rho;

    pt2d_img.y = x * c + y * d + xc;
    pt2d_img.x = x * e + y + yc;
  } else {
    pt2d_img.x = yc;
    pt2d_img.y = xc;
  }
  return pt2d_img;
}

cv::Point3d
DavideScaramCameraDistortionModel::UnProject(const cv::Point2d &point2d) {
  // transform to camera coordinates
  cv::Point3d pt2d_img;
  double c = affine_param_.val[0];
  double d = affine_param_.val[1];
  double e = affine_param_.val[2];
  double xc = center_[1];
  double yc = center_[0];
  double *pol = ploy_param_;
  double invdet =
      1 / (c - d * e); // 1/det(A), where A = [c,d;e,1] as in the Matlab file

  double xp = invdet * ((point2d.y - xc) - d * (point2d.x - yc));
  double yp = invdet * (-e * (point2d.y - xc) + c * (point2d.x - yc));

  double r =
      sqrt(xp * xp +
           yp * yp); // distance [pixels] of  the point from the image center
  double zp = pol[0];
  double r_i = 1;
  int i;

  for (i = 1; i < 5; i++) {
    r_i *= r;
    zp += r_i * pol[i];
  }

  // normalize to unit norm
  double invnorm = 1 / sqrt(xp * xp + yp * yp + zp * zp);

  pt2d_img.x = invnorm * xp;
  pt2d_img.y = invnorm * yp;
  pt2d_img.z = invnorm * zp;
  return pt2d_img;
}

cv::Point2d
DavideScaramCameraDistortionModel::distort(const cv::Point2d &undistort_pt) {
  double Nxc = height_ / 2.0;
  double Nyc = width_ / 2.0;
  double Nz = -(double)width_ / (double)SF;
  cv::Point3d pw;
  pw.x = (undistort_pt.y - Nxc);
  pw.y = (undistort_pt.x - Nyc);
  pw.z = Nz;

  cv::Point2d distort_pt = Project(pw);
  return distort_pt;
}

cv::Point2d
DavideScaramCameraDistortionModel::undistort(const cv::Point2d &distort_pt) {
  LOG(ERROR) << "TODO";
  //            cv::Point3d pw = UnProject(distort_pt);
  //            double Nxc = height_/2.0;
  //            double Nyc = width_/2.0;
  //            cv::Point2d undistort_pt;
  //            undistort_pt.x = pw.z*pw.x*Nxc;
  ////            undistort_pt.x = pw.x + Nxc;
  ////            undistort_pt.y = pw.y + Nyc;
  return cv::Point2d();
}

} // namespace base
} // namespace minieye