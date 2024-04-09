//
// Created by minieye on 2022/6/1.
//

#include "aipilot/services/dfg_avm_mapping/camera/cmei_model.h"

#include "aipilot/services/dfg_avm_mapping/camera/pinhole_model.h"
#include "glog/logging.h"

namespace minieye {
namespace base {
cv::Point2d CMeiCameraDistortionModel::Project(const cv::Point3d &point3d) {
  cv::Point2d pt2d_img;
  if (std::isless(point3d.z, 0.f)) {
    LOG(ERROR) << "The input point (" << point3d
               << ") should be in front of the camera";
    pt2d_img.x = DBL_MAX;
    pt2d_img.y = DBL_MAX;
  } else {
    cv::Vec3d Xc(point3d.x, point3d.y, point3d.z);
    double xi = distort_params_.val[4];
    double k1 = distort_params_.val[0];
    double k2 = distort_params_.val[1];
    double p1 = distort_params_.val[2];
    double p2 = distort_params_.val[3];
    double fx = intrinsic_params_.val[0];
    double fy = intrinsic_params_.val[4];
    double cx = intrinsic_params_.val[2];
    double cy = intrinsic_params_.val[5];
    double s = intrinsic_params_.val[1];
    // convert to unit sphere
    cv::Vec3d Xs = Xc / cv::norm(Xc);

    // convert to normalized image plane
    cv::Vec2d xu = cv::Vec2d(Xs[0] / (Xs[2] + xi), Xs[1] / (Xs[2] + xi));

    // add distortion
    cv::Vec2d xd;
    double r2 = xu[0] * xu[0] + xu[1] * xu[1];
    double r4 = r2 * r2;

    xd[0] = xu[0] * (1 + k1 * r2 + k2 * r4) + 2 * p1 * xu[0] * xu[1] +
            p2 * (r2 + 2 * xu[0] * xu[0]);
    xd[1] = xu[1] * (1 + k1 * r2 + k2 * r4) + p1 * (r2 + 2 * xu[1] * xu[1]) +
            2 * p2 * xu[0] * xu[1];

    // convert to pixel coordinate
    pt2d_img.x = fx * xd[0] + s * xd[1] + cx;
    pt2d_img.y = fy * xd[1] + cy;
  }
  return pt2d_img;
}

std::shared_ptr<BaseCameraModel> CMeiCameraDistortionModel::get_camera_model() {
  std::shared_ptr<PinholeCameraModel> camera_model(new PinholeCameraModel());
  camera_model->set_width(width_);
  camera_model->set_height(height_);
  camera_model->set_intrinsic_params(intrinsic_params_);

  return std::dynamic_pointer_cast<BaseCameraModel>(camera_model);
}

bool CMeiCameraDistortionModel::set_params(size_t width, size_t height,
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
  if (std::max(distort_params.rows, distort_params.cols) == 5) {
    memcpy(intrinsic_params_.val, intrins_.data, sizeof(double) * 9);
    memcpy(distort_params_.val, distort_.data, sizeof(double) * 5);
    ret = true;
  } else {
    LOG(ERROR) << "wrong camera model distort coeff";
  }

  return ret;
}

cv::Point3d CMeiCameraDistortionModel::UnProject(const cv::Point2d &point2d) {
  // transform to image coordinates
  double xi = distort_params_.val[4];
  double k1 = distort_params_.val[0];
  double k2 = distort_params_.val[1];
  double p1 = distort_params_.val[2];
  double p2 = distort_params_.val[3];
  double fx = intrinsic_params_.val[0];
  double fy = intrinsic_params_.val[4];
  double cx = intrinsic_params_.val[2];
  double cy = intrinsic_params_.val[5];
  double s = intrinsic_params_.val[1];
  //            double x, y;
  //            x = (point2d.x - cx) / fx;
  //            y = (point2d.y - cy) / fy;

  cv::Vec2d pi(point2d.x, point2d.y); // image point
  cv::Vec2d pp((pi[0] * fy - cx * fy - s * (pi[1] - cy)) / (fx * fy),
               (pi[1] - cy) / fy); // plane
  cv::Vec2d pu = pp;               // points without distortion

  // remove distortion iteratively by zq
  const double EPS = 1e-8;
  for (int j = 0; j < 1000; j++) {
    double r2 = pu[0] * pu[0] + pu[1] * pu[1];
    double r4 = r2 * r2;
    double fx = (1 + k1 * r2 + k2 * r4) * pu[0] + 2 * p1 * pu[0] * pu[1] +
                p2 * (r2 + 2 * pu[0] * pu[0]);
    double fy = (1 + k1 * r2 + k2 * r4) * pu[1] + 2 * p2 * pu[0] * pu[1] +
                p1 * (r2 + 2 * pu[1] * pu[1]);
    double fxdx = (1 + k1 * r2 + k2 * r4) + 2 * p1 * pu[1] + 6 * p2 * pu[0] +
                  (2 * k1 + 4 * k2 * r2) * pu[0] * pu[0];
    double fydy = (1 + k1 * r2 + k2 * r4) + 2 * p2 * pu[0] + 6 * p1 * pu[1] +
                  (2 * k1 + 4 * k2 * r2) * pu[1] * pu[1];
    double fix_x = (fx - pp[0]) / fxdx;
    double fix_y = (fy - pp[1]) / fydy;
    pu[0] = pu[0] - fix_x;
    pu[1] = pu[1] - fix_y;

    if ((fabs(fix_x) < EPS) && (fabs(fix_y) < EPS)) {
      break;
    }
  }

  // project to unit sphere by zq
  double r2 = pu[0] * pu[0] + pu[1] * pu[1];
  double a = (r2 + 1);
  double b = 1.0 - (xi * xi);
  double Zs = ((xi + sqrt(1 + b * r2)) / a) - xi;
  cv::Vec3d Xw = cv::Vec3d(pu[0] * (Zs + xi), pu[1] * (Zs + xi), Zs);
  return cv::Point3d(Xw[0], Xw[1], Xw[2]);
}

cv::Point2d
CMeiCameraDistortionModel::distort(const cv::Point2d &undistort_pt) {
  const double fx = intrinsic_params_(0, 0);
  const double fy = intrinsic_params_(1, 1);
  const double cx = intrinsic_params_(0, 2);
  const double cy = intrinsic_params_(1, 2);

  const double *d = (double *)distort_params_.val;
  cv::Vec2d f = cv::Vec2d(fx, fy);
  cv::Vec2d c = cv::Vec2d(cx, cy);
  cv::Vec2d k = cv::Vec2d(d[0], d[1]);
  cv::Vec2d p = cv::Vec2d(d[2], d[3]);
  double s = intrinsic_params_(0, 1);
  double xi = distort_params_.val[4];

  // pix -> camera
  cv::Vec3d pt_vec = cv::Vec3d(undistort_pt.x, undistort_pt.y, 1);
  pt_vec = ((cv::Matx33d)intrinsic_params_).inv() * pt_vec;
  pt_vec /= pt_vec[2];

  double x = pt_vec[0];
  double y = pt_vec[1];
  double w = pt_vec[2];
  // camera->distort
  double r = std::sqrt(x * x + y * y + w * w);
  double Xs = x / r;
  double Ys = y / r;
  double Zs = w / r;
  double xu = Xs / (Zs + xi);
  double yu = Ys / (Zs + xi);
  double r2 = xu * xu + yu * yu;
  double r4 = r2 * r2;
  double xd = (1 + k[0] * r2 + k[1] * r4) * xu + 2 * p[0] * xu * yu +
              p[1] * (r2 + 2 * xu * xu);
  double yd = (1 + k[0] * r2 + k[1] * r4) * yu + p[0] * (r2 + 2 * yu * yu) +
              2 * p[1] * xu * yu;

  double u = f[0] * xd + s * yd + c[0];
  double v = f[1] * yd + c[1];
  return cv::Point2f(u, v);
}

cv::Point2d
CMeiCameraDistortionModel::undistort(const cv::Point2d &distort_pt) {
  cv::Point3d unit_plane_pt = UnProject(distort_pt);
  cv::Vec3d Xw(unit_plane_pt.x, unit_plane_pt.y, unit_plane_pt.z);
  Xw = intrinsic_params_ * Xw;
  cv::Vec3d ppu = cv::Vec3d(Xw[0] / Xw[2], Xw[1] / Xw[2], 1.0);
  // transform to image coordinates
  return cv::Point2d(ppu[0], ppu[1]);
}
} // namespace base
} // namespace minieye