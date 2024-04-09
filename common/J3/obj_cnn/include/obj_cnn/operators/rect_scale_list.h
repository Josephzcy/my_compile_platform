// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_RECT_SCALE_LIST_H_
#define INCLUDE_OBJ_CNN_OPERATORS_RECT_SCALE_LIST_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

// Scale a rectangle.
class RectScaleListOperator : public Operator {
 public:
  // Ctor
  RectScaleListOperator();

  // Dtor
  virtual ~RectScaleListOperator() {}

 private:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "RectScaleList"; }

  int LoadOption(toml::value const &) override;

 private:
  float scale_w_;
  float scale_h_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_RECT_SCALE_LIST_H_
