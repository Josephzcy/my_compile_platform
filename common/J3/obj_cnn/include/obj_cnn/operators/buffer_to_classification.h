// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_BUFFER_TO_CLASSIFICATION_H_
#define INCLUDE_OBJ_CNN_OPERATORS_BUFFER_TO_CLASSIFICATION_H_

#include <memory>
#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"

namespace obj_cnn {

class BufferToClassificationOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "BufferToClassification"; }

  enum BufferType {
    kFloat,
    kUInt32,
  };

  // buffer-type
  int LoadOption(toml::value const &) override;

 private:
  BufferType buffer_type_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_BUFFER_TO_CLASSIFICATION_H_
