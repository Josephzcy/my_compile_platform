#include "./apa_common/vehicle_info.h"
#include <fstream>
#include <string>
#include "apa_interface.h"
#include "glog/logging.h"
#include "json.hpp"

namespace apa {
void VehicleInfoClass::Init(const std::string& vehicle_config_file) {
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
VehicleInfo VehicleInfoClass::GetResult() {
  if (!m_initialized) {
    LOG(ERROR) << "no initialized.";
  }
  return m_result;
};

}  // namespace apa