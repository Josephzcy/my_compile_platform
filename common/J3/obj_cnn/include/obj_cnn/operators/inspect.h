// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_INSPECT_H_
#define INCLUDE_OBJ_CNN_OPERATORS_INSPECT_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

// Show infomations of input data, without any output.
class InspectOperator : public Operator {
 private:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "Inspect"; }

  int LoadOption(toml::value const &) override;

 private:
  std::string out_dir_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_INSPECT_H_
