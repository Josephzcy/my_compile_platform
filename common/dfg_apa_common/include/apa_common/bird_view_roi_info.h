// Copyright [2019] - MiniEye INC.
#pragma once
#include <msgpack.hpp>
#include "common/base/singleton.h"
#include "opencv2/opencv.hpp"
#include "apa_interface.h"
namespace apa {

class BirdViewROIClass : public SingletonBase<BirdViewROIClass> {
 public:
  friend class SingletonBase<BirdViewROIClass>;

  void Init(const std::string& vehicle_config_file);
  BirdViewROI GetResult();
  void World2Raw(cv::Point2f world_pt,cv::Point2f *result);
  void Raw2World(cv::Point2f raw_pt,cv::Point2f *result);
 private:
  BirdViewROIClass() : m_initialized(false) {}
  virtual ~BirdViewROIClass() {}

 private:
  bool m_initialized;
  BirdViewROI m_result;
};
}  // namespace apa