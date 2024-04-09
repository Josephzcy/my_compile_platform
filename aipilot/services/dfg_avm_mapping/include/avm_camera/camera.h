//
// Created by minieye on 2022/6/1.
//

#ifndef AIPLORER_CAMERA_H
#define AIPLORER_CAMERA_H

#include <memory>
#include <string>

#include "opencv2/opencv.hpp"

namespace minieye {
namespace base {
class BaseCameraModel {
public:
  virtual ~BaseCameraModel() = default;

  virtual cv::Point2d Project(const cv::Point3d &point3d) = 0;

  virtual cv::Point3d UnProject(const cv::Point2d &point2d) = 0;

  virtual std::string name() const = 0;

  inline void set_width(size_t width) { image_width_ = width; }

  inline void set_height(size_t height) { image_height_ = height; }

  inline size_t get_width() const { return image_width_; }

  inline size_t get_height() const { return image_height_; }

protected:
  size_t image_width_ = 0;
  size_t image_height_ = 0;
};

class BaseCameraDistortionModel {
public:
  BaseCameraDistortionModel() = default;

  virtual ~BaseCameraDistortionModel() = default;
  /**
   * 将3d相机坐标点投影到2d像素坐标
   * @param point3d，3d point in camera space;
   *                 the input point should be in front of the camera, point3d.z
   *> 0
   * @return 2d point in pix plane
   **/
  virtual cv::Point2d Project(const cv::Point3d &point3d) = 0;

  /**
   * 将像素坐标逆变换到相机坐标系
   * @param point2d，像素坐标
   * @return
   */
  virtual cv::Point3d UnProject(const cv::Point2d &point2d) = 0;

  virtual cv::Point2d distort(const cv::Point2d &undistort_pt) = 0;

  virtual cv::Point2d undistort(const cv::Point2d &distort_pt) = 0;

  virtual std::shared_ptr<BaseCameraModel> get_camera_model() = 0;

  virtual std::string name() const = 0;

  /**
   *
   * @param width
   * @param height
   * @param intrins_params, 如果大小为2,表示 Scaram 模型的 center-2,
   * @param distort_params，设置畸变模型的时候，k1,k2,k3，p1,p2，
   *                      如果是多项式畸变模型，则为k0,k1,k2,k3,k4，一般k0=1
   *                      如果是 Scaram 模型，则为 ploy-5, affine-4, invploy
   * @return
   */
  virtual bool set_params(size_t width, size_t height,
                          const cv::Mat &intrins_params,
                          const cv::Mat &distort_params) = 0;

  size_t get_height() const { return height_; }

  size_t get_width() const { return width_; }

protected:
  size_t width_ = 0;
  size_t height_ = 0;
};
} // namespace base
} // namespace minieye

#endif // AIPLORER_CAMERA_H
