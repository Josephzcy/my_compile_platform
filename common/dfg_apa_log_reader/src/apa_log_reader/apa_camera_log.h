// Copyright [2019] - MiniEye INC.
#pragma once
#include <string>
#include <fstream>
#include <vector>

namespace log_reader {

class ApaCameraLog {
 public:
    ApaCameraLog();
    virtual ~ApaCameraLog();
    bool Init(const std::string& log_file);
    bool MatchTimestamp(const uint32_t& frame_id, uint64_t* tm_ms);

 private:
    std::ifstream m_log_ifs;
};

}  // namespace log_reader
