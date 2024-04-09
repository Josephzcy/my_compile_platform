// Copyright 2023 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_PED_CLASSIFICATION_REGRESS_H_
#define INCLUDE_OBJ_CNN_OPERATORS_PED_CLASSIFICATION_REGRESS_H_

#include <memory>
#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"

namespace obj_cnn {
class PedClassificationRegressOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "PedClassificationRegress"; }

  // image-size
  //   长度 2 的数组，表示整图的大小。
  //
  // resize-size
  //   长度 2 的数组，表示 ROI 缩放到的大小。
  int LoadOption(toml::value const &) override;

  struct PaddedSize {
    size_t begin;
    size_t size;
    size_t end;

    size_t total() const { return begin + size + end; }
  };

  enum RightBottomType {
    kSize,    // width, height
    kPoint,   // offset from left-top
    kOffset,  // offset from right-bottom
  };

 private:
  size_t classifications_;
  cv::Size image_size_;
  cv::Size resize_size_;
  // RightBottomType right_bottom_type_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_PED_CLASSIFICATION_REGRESS_H_
