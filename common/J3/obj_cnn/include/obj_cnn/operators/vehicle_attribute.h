// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_ATTRIBUTE_H_
#define INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_ATTRIBUTE_H_

#include <memory>
#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"

namespace obj_cnn {

class VehicleAttributeOperator : public Operator {
 private:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "VehicleAttribute"; }

  int LoadOption(toml::value const &) override;

 public:
  VehicleAttributeOperator();
  virtual ~VehicleAttributeOperator();

 private:
  float near_width_;
  float expand_scale_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_ATTRIBUTE_H_
