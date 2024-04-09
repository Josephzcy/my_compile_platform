// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_IMAGE_CONCAT_H_
#define INCLUDE_OBJ_CNN_OPERATORS_IMAGE_CONCAT_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

class ImageConcatOperator : public OperatorBinary {
 public:
  int LoadOption(toml::value const &conf) override;

  std::string name() const override { return "ImageConcat"; }

  int Operate(CvData const &, CvData const &, CvData &output) override;

 private:
  std::vector<uint8_t> buffer_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_IMAGE_CONCAT_H_
