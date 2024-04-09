// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_ATTRIBUTE_BATCH_H_
#define INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_ATTRIBUTE_BATCH_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

// Show infomations of input data, without any output.
class VehicleAttributeBatchOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "VehicleAttributeBatch"; }

  int LoadOption(toml::value const &) override;

  virtual ~VehicleAttributeBatchOperator();
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_ATTRIBUTE_BATCH_H_
