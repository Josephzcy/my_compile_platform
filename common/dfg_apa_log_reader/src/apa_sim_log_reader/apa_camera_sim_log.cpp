// Copyright [2019] - MiniEye INC.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "apa_camera_sim_log.h"

namespace sim_log_reader {

ApaCameraLog::ApaCameraLog() {
}

ApaCameraLog::~ApaCameraLog() {
}

bool ApaCameraLog::Init(const std::string& log_file) {
    m_log_ifs.open(log_file.c_str(), std::ifstream::in);
    if (!m_log_ifs.good()) {
        std::cerr << "open log file failed: " << log_file << std::endl;
        return false;
    } else {
        std::cerr << "parse apa camera log file : " << log_file << std::endl;
        return true;
    }
}

bool ApaCameraLog::MatchTimestamp(const uint32_t& frame_id, uint64_t* tm_ms) {
    if (!m_log_ifs.good()) {
        return false;
    }
    std::streampos pos;
    uint32_t id_read = 0;
    std::string line_log = "";
    while (!m_log_ifs.eof() && id_read <= frame_id) {
        pos = m_log_ifs.tellg();
        getline(m_log_ifs, line_log);
        std::stringstream ss_log;
        ss_log.str(line_log);
        uint64_t seconds = 0;
        uint64_t micro_seconds = 0;
        std::string vedio_name = "";
        ss_log >> seconds >> micro_seconds >> vedio_name >> id_read;
        if (id_read == frame_id) {
            *tm_ms = seconds*1000 + micro_seconds / 1000;
        }
    }
    m_log_ifs.seekg(pos);
    return true;
}

}  // namespace sim_log_reader
