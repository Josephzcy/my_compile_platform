#include "./apa_common/bird_view_roi_info.h"
#include <fstream>
#include <string>
#include "apa_interface.h"
#include "glog/logging.h"
#include "json.hpp"

namespace apa {
void BirdViewROIClass::Init(const std::string& vehicle_config_file) {
  if (m_initialized) {
    LOG(ERROR) << "already initialized.";
  }
  nlohmann::json config_json;
  std::ifstream config_stream(vehicle_config_file, std::ifstream::in);
  config_stream >> config_json;

  std::vector<uint8_t> v_msgpack = nlohmann::json::to_msgpack(config_json);
  const char* ptr = reinterpret_cast<const char*>(&v_msgpack[0]);
  msgpack::object_handle oh = msgpack::unpack(ptr, v_msgpack.size());
  msgpack::object obj = oh.get();
  obj.convert(m_result);
  config_stream.close();
  m_initialized = true;
}
BirdViewROI BirdViewROIClass::GetResult() {
  if (!m_initialized) {
    LOG(ERROR) << "no initialized.";
  }
  return m_result;
}


void BirdViewROIClass::World2Raw(cv::Point2f world_pt, cv::Point2f* result) {
  // result->x = (world_pt.x - m_result.startx) / m_result.xres;
  // result->y = (m_result.endy - world_pt.y) / m_result.yres;
    result->x = world_pt.x / 39.65 + 352 / 2;
    result->y = 704 / 2 - world_pt.y / 39.65;
}
void BirdViewROIClass::Raw2World(cv::Point2f raw_pt, cv::Point2f* result) {
  result->x = (raw_pt.x - 352 /2) * m_result.xres;
  result->y = (704 / 2 - raw_pt.y) * m_result.yres;
}


};  // namespace apa
