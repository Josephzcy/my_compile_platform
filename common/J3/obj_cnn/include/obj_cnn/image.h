// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_IMAGE_H_
#define INCLUDE_OBJ_CNN_IMAGE_H_

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "camera.pb.h"
#include "logfacade/logfacade.h"
#include "opencv2/core/mat.hpp"

namespace obj_cnn {

// Contains things we need in minieye::CameraFrame.
//
struct MinieyeCameraFrame {
  uint32_t camera_id{};                    // 1
  uint64_t timestamp{};                    // 2
  uint64_t tick{};                         // 3
  uint64_t frame_id{};                     // 4
  std::vector<uint64_t> image_plane_addr;  // 5
  uint32_t image_width{};                  // 6
  uint32_t image_height{};                 // 7
  uint32_t stride{};                       // 8
  uint32_t image_type{};                   // 9
  std::vector<uint8_t> image_supplement;   // 10
  // We do not need extended infomation
  // extended_info // 11
};

enum class ImageType {
  YuvI420,
  YuvNv12,
  Yuv444,
  Gray,
  Bgr,
  NotImage,
};

inline int ImageTypeFromStr(std::string const &image_type_str,
                            ImageType *image_type) {
  if (image_type_str == "gray") {
    *image_type = ImageType::Gray;
    return 0;
  }

  if (image_type_str == "bgr") {
    *image_type = ImageType::Bgr;
    return 0;
  }

  if (image_type_str == "nv12") {
    *image_type = ImageType::YuvNv12;
    return 0;
  }

  if (image_type_str == "i420") {
    *image_type = ImageType::YuvI420;
    return 0;
  }

  if (image_type_str == "yuv444") {
    *image_type = ImageType::Yuv444;
    return 0;
  }

  LOGF(ERROR, "undefined image type %s", image_type_str.c_str());
  return 1;
}

inline char const *ImageTypeToStr(ImageType image_type) {
  switch (image_type) {
    case ImageType::Bgr:
      return "bgr";
    case ImageType::YuvNv12:
      return "nv12";
    case ImageType::YuvI420:
      return "i420";
    case ImageType::Yuv444:
      return "yuv444";
    case ImageType::Gray:
      return "gray";
    case ImageType::NotImage:
      return "not-image";
    default:
      return "unexpected-image-type";
  }
}

// Abstraction of a image, maybe a batch of images.
class Image {
 public:
  // Image use pimpl idiom to hide fields and details.
  class Impl;

  // Create a empty image, in that case image_type() return ImageType::NotImage.
  Image();

  // Create a image from a detail class.
  explicit Image(std::shared_ptr<Impl> impl) : impl_(impl) {}

  // Width of image.
  size_t width() const;

  // Height of image.
  size_t height() const;

  // Batch size of image, if implementation do not support batch size, this
  // always return 1.
  size_t batch() const;

  // Copy data into a cv::Mat.
  //
  // This might be expensive, try only use it for debug codes.
  int CopyToCvMat(cv::Mat &) const;

  // Copy the i-th image data into a cv::Mat.
  //
  // This might be expensive, try only use it for debug codes.
  int CopyBatchToCvMat(size_t i, cv::Mat &) const;

  // Copy data from cv::Mat.
  int CopyFromCvMat(cv::Mat const &, ImageType);

  // minieye::CameraFrame usually contains pointers of image data and image
  // shape infomation. This function load those data to Image's inner type.
  //
  // If some profile use different implementions to loader image, loader option
  // can be used to decide which loader to be used. Following is valid loader
  // for each profile.
  //
  // mdc:
  // - adsfi
  // - dds
  // - default (alias of dds)
  //
  // bionic:
  // - dds
  // - default (alias of dds)
  //
  // tda4
  // - dds
  // - default (alias of dds)
  int LoadFromProtoHeader(minieye::CameraFrame const &cf,
                          std::string const &loader) {
    MinieyeCameraFrame mcf;

    mcf.camera_id = cf.camera_id();  // 1
    mcf.timestamp = cf.timestamp();  // 2
    mcf.tick = cf.tick();            // 3
    mcf.frame_id = cf.frame_id();    // 4

    // 5
    mcf.image_plane_addr = {cf.image_plane_addr().begin(),
                            cf.image_plane_addr().end()};

    mcf.image_width = cf.image_width();    // 6
    mcf.image_height = cf.image_height();  // 7
    mcf.stride = cf.stride();              // 8
    mcf.image_type = cf.image_type();      // 9

    // 10
    mcf.image_supplement = {cf.image_supplement().begin(),
                            cf.image_supplement().end()};
    return LoadFromProtoHeader(mcf, loader);
  }

  int LoadFromProtoHeader(MinieyeCameraFrame const &,
                          std::string const &loader);

  // 获取与 Profile 有关的 Image 的实现。
  std::shared_ptr<Impl> impl() const { return impl_; }

  ImageType image_type() const;

  // Get the start data pointer of image.
  uint8_t *data();

 private:
  std::shared_ptr<Impl> impl_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_IMAGE_H_
