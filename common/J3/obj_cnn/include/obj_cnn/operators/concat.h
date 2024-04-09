// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_CONCAT_H_
#define INCLUDE_OBJ_CNN_OPERATORS_CONCAT_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

// 将多个 CvData::List 合并为一个更长的 CvData::List。
class ConcatOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "Concat"; }

  // 没有选项。
  int LoadOption(toml::value const &) override { return 0; }
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_CONCAT_H_
