// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_RELOCATE_OBJECT_H_
#define INCLUDE_OBJ_CNN_OPERATORS_RELOCATE_OBJECT_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

// 视觉算法如果接受了整图（G）上的 ROI（R）缩放后的图像（S）作为输入，并且输出
// 了定位在 S 上的框目标（O）。
//
// 此 Operator 利用 ROI，G 的大小和 S 的大小，才能将 O 重新定位到 G 上。
class RelocateObjectOperator : public OperatorBinary {
 public:
  // 目前此 Operator 只用在检测的重新定位，所以假设 detections 的形状是：
  //
  //   [ [ Rectangle Confidence HitType ],
  //     [ Rectangle Confidence HitType ],
  //     ...
  //     [ Rectangle Confidence HitType ] ]
  int Operate(CvData const &roi, CvData const &detections,
              CvData &output) override;

  std::string name() const override { return "RelocateObject"; }

  // image-size
  //   长度 2 的数组，表示整图的大小。
  //
  // resize-size
  //   长度 2 的数组，表示 ROI 缩放到的大小。
  int LoadOption(toml::value const &) override;

 private:
  cv::Size image_size_;
  cv::Rect resize_roi_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_RELOCATE_OBJECT_H_
