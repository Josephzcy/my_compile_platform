// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_ANCHOR_DETECTION_H_
#define INCLUDE_OBJ_CNN_OPERATORS_ANCHOR_DETECTION_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "obj_cnn/operator.h"
#include "obj_cnn/sigmoid_map.h"

namespace obj_cnn {

struct DspAnchorDetectionInput;

// This is a operator to parse detection output based on anchor box.
class AnchorDetectionOperator : public Operator {
 public:
  AnchorDetectionOperator() {}

  virtual ~AnchorDetectionOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "AnchorDetection"; }

  enum class ChannelLayout {
    AnchorFirst = 0,
    RegThenCls,
    RegClsIndependent,
    Invalid,
  };

  int LoadOption(toml::value const &conf) override;

 private:
  int LoadOptionDsp(toml::value const &conf);

  int OperateDsp(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output);

  int SetDspInput(DspAnchorDetectionInput * input);

  // Configrations
  size_t classcification_count_;
  cv::Size image_size_;
  std::vector<std::vector<cv::Size2f>> anchor_box_;
  std::vector<size_t> stride_;
  bool box3d_;
  size_t expected_operator_input_size_each_stride_;

  // 模型目标置信度大于此值的时候产生一个目标响应。
  float min_confidence_;

  // 不为输出数据进行 sigmoid。
  bool no_sigmoid_;

  // 将检测到的结果进行局部非最大抑制。
  bool nms_;
  float overlap_thres_;

  // enable_type_[i] 为 false 时，索引为 i 的类型不会输出。
  std::vector<bool> enable_type_;

  // 将检测目标划分到 rois_ 给定的 ROI 上。
  bool split_by_roi_;

  // 缩放后的图像上的 ROI。
  std::vector<cv::Rect> rois_;

  size_t output_size_;

  // 重新定位目标到缩放前的图像上。
  bool relocate_;

  // 原图像上的 ROI。
  std::vector<cv::Rect> image_rois_;

  // 使用 DSP 进行后处理，只有在 TDA4 有效。
  bool use_dsp_;

  uint8_t* src_buffer_;
  uint8_t* dst_buffer_;

  uint8_t dsp_target_;

  // Help calculator.
  SigmoidMap sigmoid_map_;

  // 内存布局。
  ChannelLayout channel_layout_;

  std::vector<std::vector<ObjectDetectionTypedSet>> type_set_cache1_;
  std::vector<std::vector<ObjectDetectionTypedSet>> type_set_cache2_;

  // 隐藏私有函数。
  class Private;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_ANCHOR_DETECTION_H_
