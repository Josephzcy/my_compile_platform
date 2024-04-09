// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_PLAIN_DETECTION_H_
#define INCLUDE_OBJ_CNN_OPERATORS_PLAIN_DETECTION_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "obj_cnn/operator.h"
#include "toml/value.hpp"

namespace obj_cnn {

/*! Load detection result from binary outputs of network, while output is
 * "plain" data.
 *
 * MDC transformed model of YOLO-v5 detetion can parse result of network in
 * network processor, this operator is used to load those parse result.
 */
class PlainDetectionOperator : public Operator {
 public:
  PlainDetectionOperator() {}

  virtual ~PlainDetectionOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "PlainDetection"; }

  enum class CamPos {
    Front = 0,
    Side,
    Rear,
    Invalid,
  };

  int LoadOption(toml::value const &conf) override;

 private:
  void GetIdx(const int &total_idx, int &id, int &stride_idx,
              int &anchor_box_idx);

  size_t height_;
  size_t stride_;
  size_t classcification_count_;
  float min_confidence_;

  bool box3d_;
  size_t expected_operator_input_size_;
  std::vector<std::vector<cv::Size2f>> anchor_box_;
  std::vector<cv::Size> stride_image_size_;
  std::vector<size_t> stride_box_{8, 16, 32};
  cv::Size image_size_;
  CamPos cam_pos_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_PLAIN_DETECTION_H_
