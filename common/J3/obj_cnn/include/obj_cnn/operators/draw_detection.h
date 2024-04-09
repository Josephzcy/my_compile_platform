// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_DRAW_DETECTION_H_
#define INCLUDE_OBJ_CNN_OPERATORS_DRAW_DETECTION_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

class DrawDetectionOperator : public Operator {
 public:
  DrawDetectionOperator() {}
  virtual ~DrawDetectionOperator() {}

  int Operate(std::vector<CvData const *> const &inputs,
              std::vector<CvData *> const &outputs) override;

  std::string name() const override { return "DrawDetection"; }

  int LoadOption(toml::value const &conf) override { return 0; }

 private:
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_DRAW_DETECTION_H_
