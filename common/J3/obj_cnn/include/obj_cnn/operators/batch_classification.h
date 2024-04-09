// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_BATCH_CLASSIFICATION_H_
#define INCLUDE_OBJ_CNN_OPERATORS_BATCH_CLASSIFICATION_H_

#include <memory>
#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"

namespace obj_cnn {
class BatchClassificationOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "BatchClassification"; }

  // 没有选项。
  int LoadOption(toml::value const &) override { return 0; }
};
}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_BATCH_CLASSIFICATION_H_
