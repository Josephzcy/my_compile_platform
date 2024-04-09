// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_CONCAT_BUFFER_H_
#define INCLUDE_OBJ_CNN_OPERATORS_CONCAT_BUFFER_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

// 将多个 Buffer 合并为一个更长的 Buffer。
class ConcatBufferOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "ConcatBuffer"; }

  // 没有选项。
  int LoadOption(toml::value const &) override { return 0; }

 private:
  std::vector<uint8_t> buffer_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_CONCAT_BUFFER_H_
