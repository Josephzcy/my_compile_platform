// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_COLLECT_INTO_LIST_H_
#define INCLUDE_OBJ_CNN_OPERATORS_COLLECT_INTO_LIST_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

class CollectIntoListOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "CollectIntoList"; }

  int LoadOption(toml::value const &) override { return 0; }
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_COLLECT_INTO_LIST_H_
