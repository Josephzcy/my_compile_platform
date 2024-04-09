// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_RECT_SCALE_H_
#define INCLUDE_OBJ_CNN_OPERATORS_RECT_SCALE_H_

#include <string>

#include "obj_cnn/operator.h"

namespace obj_cnn {

// Scale a rectangle.
class RectScaleOperator : public OperatorUnary {
 public:
  // Ctor
  RectScaleOperator();

  // Dtor
  virtual ~RectScaleOperator();

 private:
  int Operate(CvData const &roi, CvData &roi_output) override;

  std::string name() const override { return "RectScale"; }

  int LoadOption(toml::value const &) override;

 private:
  float scale_w_;
  float scale_h_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_RECT_SCALE_H_
