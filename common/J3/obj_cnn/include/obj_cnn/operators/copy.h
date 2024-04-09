// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_COPY_H_
#define INCLUDE_OBJ_CNN_OPERATORS_COPY_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

class CopyOperator : public Operator {
 public:
  CopyOperator() {}
  virtual ~CopyOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "Copy"; }
  int LoadOption(toml::value const &) override { return 0; }
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_COPY_H_
