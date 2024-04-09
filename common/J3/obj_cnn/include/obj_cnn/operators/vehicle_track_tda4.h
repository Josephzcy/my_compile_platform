// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_TRACK_TDA4_H_
#define INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_TRACK_TDA4_H_

#include <memory>
#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"

namespace obj_cnn {

class VehicleTrackTda4Operator : public Operator {
 public:
  VehicleTrackTda4Operator();

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "VehicleTrackTda4"; }

  int LoadOption(toml::value const &) override { return 0; };

 private:
  std::vector<uint8_t> template_buffer_;
  std::shared_ptr<BufferView> template_buffer_view_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_TRACK_TDA4_H_
