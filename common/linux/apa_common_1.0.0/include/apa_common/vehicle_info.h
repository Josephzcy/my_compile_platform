// Copyright [2019] - MiniEye INC.
#pragma once
#include <msgpack.hpp>
#include "common/base/singleton.h"
#include "opencv2/opencv.hpp"

namespace apa {

struct VehicleInfo {
  float start_x;  // left
  float start_y;  // rear
  float end_x;    // right
  float end_y;    // front
  MSGPACK_DEFINE_MAP(start_x,start_y,end_x,end_y);
};
class VehicleInfoClass : public SingletonBase<VehicleInfoClass> {
 public:
  friend class SingletonBase<VehicleInfoClass>;

  void Init(const std::string& vehicle_config_file);
  VehicleInfo GetResult();
  
 private:
  VehicleInfoClass() : m_initialized(false) {}
  virtual ~VehicleInfoClass() {}

 private:
  bool m_initialized;
  VehicleInfo m_result;
};
}  // namespace apa