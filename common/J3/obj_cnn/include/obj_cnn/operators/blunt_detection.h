// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_BLUNT_DETECTION_H_
#define INCLUDE_OBJ_CNN_OPERATORS_BLUNT_DETECTION_H_

#include <logfacade/logfacade.h>
#include <obj_cnn/operator.h>
#include <obj_cnn/operator_graph_context.h>
#include <obj_cnn/sigmoid_map.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace obj_cnn {

/*! Parse YOLOv5 detection output and hard-coded most options.

  This operator exists because of this need a context to load a detection
  shape. */
class BluntDetectionOperator : public Operator {
 public:
  BluntDetectionOperator() {}

  virtual ~BluntDetectionOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "BluntDetection"; }

  int LoadOptionWithContext(OperatorGraphContext *context,
                            toml::value const &conf) override;

  int LoadOption(toml::value const &conf) override;

 private:
  enum class ChannelLayout {
    AnchorFirst = 0,
    RegThenCls,
    Invalid,
  };

  // Configrations
  size_t classcification_count_;
  cv::Size image_size_;
  std::vector<cv::Size> anchor_box_;
  size_t stride_;
  float min_confidence_;
  bool no_sigmoid_;

  SigmoidMap sigmoid_map_;
  OperatorGraphContext *context_;

  std::vector<bool> enable_type_map_;

  std::string det_nn_id_;
  std::vector<float> scale_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_BLUNT_DETECTION_H_
