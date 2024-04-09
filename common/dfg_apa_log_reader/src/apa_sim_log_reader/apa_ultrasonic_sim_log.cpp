// Copyright [2019] - MiniEye INC.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "apa_ultrasonic_sim_log.h"

namespace sim_log_reader {

    ApaUltrasonicLog::ApaUltrasonicLog() {
    }

    ApaUltrasonicLog::~ApaUltrasonicLog() {
    }

    bool ApaUltrasonicLog::Init(const std::string& log_file) {
        m_ultra_long_info.clear();
        m_ultra_short_mid_info.clear();
        m_ultra_short_side_info.clear();
        m_radar_infos.clear();
        m_pre_img_tm = 0;
        m_log_ifs.open(log_file.c_str(), std::ifstream::in);
        if (!m_log_ifs.good()) {
            std::cerr << "open log file failed: " << log_file << std::endl;
            return false;
        } else {
            std::cerr << "parse apa motion log file : "
                    << log_file << std::endl;
            bool ret = ParseLog();
            MergeLog();
            return ret;
        }
    }

    bool ApaUltrasonicLog::ReadUltrasonicData(const uint64_t& timestamp,
                            std::deque<UltrasonicRadarInfo>* radar_data) {
        radar_data->clear();
        if (!m_pre_img_tm) {
            UltrasonicRadarInfo radar_info;
            while (m_radar_infos.size() &&
                    m_radar_infos.front().tm_ms < timestamp) {
                radar_info = m_radar_infos.front();
                m_radar_infos.pop_front();
            }
            radar_data->push_back(radar_info);
            radar_data->back().tm_ms = timestamp;
        } else {
            while (m_radar_infos.size() &&
                    m_radar_infos.front().tm_ms < timestamp) {
                radar_data->push_back(m_radar_infos.front());
                m_radar_infos.pop_front();
            }
        }
        m_pre_img_tm = timestamp;
        if (radar_data->size()) {
            return true;
        } else {
            return false;
        }
    }

    bool ApaUltrasonicLog::ParseLog() {
        if (!m_log_ifs.good()) {
            return false;
        }
        std::string line_log = "";
        while (std::getline(m_log_ifs, line_log)) {
            UltraLongRadarInfo ultra_long_info;
            UltraShortRadarMidInfo ultra_short_mid_info;
            UltraShortRadarSideInfo ultra_short_side_info;
            Type type = LogType(line_log);
            // std::cout << "raw log: " << line_log << std::endl;
            switch (type) {
                case kTypeAPA:
                    ParseApaLog(line_log, &ultra_long_info);
                    m_ultra_long_info.push_back(ultra_long_info);
                    break;
                case kTypeUpaMid:
                    ParseUpaMidLog(line_log, &ultra_short_mid_info);
                    m_ultra_short_mid_info.push_back(ultra_short_mid_info);
                    break;
                case kTypeUpaSide:
                    ParseUpaSideLog(line_log, &ultra_short_side_info);
                    m_ultra_short_side_info.push_back(ultra_short_side_info);
                    break;
                default:
                    break;
            }
        }
        return true;
    }

    void ApaUltrasonicLog::ParseApaLog(const std::string& log,
                                    UltraLongRadarInfo* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float apa_fl = 0;
        float apa_fr = 0;
        float apa_rl = 0;
        float apa_rr = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp
            >> apa_fl >> apa_fr >> apa_rl >> apa_rr;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->long_dis_radar_fl = apa_fl;
        data->long_dis_radar_fr = apa_fr;
        data->long_dis_radar_bl = apa_rl;
        data->long_dis_radar_br = apa_rr;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    void ApaUltrasonicLog::ParseUpaMidLog(const std::string& log,
                                UltraShortRadarMidInfo* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float upa_mid_fl = 0;
        float upa_mid_fr = 0;
        float upa_mid_rl = 0;
        float upa_mid_rr = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp >> upa_mid_fl
            >> upa_mid_fr >> upa_mid_rl >> upa_mid_rr;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->short_dis_radar_flm = upa_mid_fl;
        data->short_dis_radar_frm = upa_mid_fr;
        data->short_dis_radar_blm = upa_mid_rl;
        data->short_dis_radar_brm = upa_mid_rr;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    void ApaUltrasonicLog::ParseUpaSideLog(const std::string& log,
                                    UltraShortRadarSideInfo* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float upa_side_fl = 0;
        float upa_side_fr = 0;
        float upa_side_rl = 0;
        float upa_side_rr = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp >> upa_side_fl
            >> upa_side_fr >> upa_side_rl >> upa_side_rr;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->short_dis_radar_fl = upa_side_fl;
        data->short_dis_radar_fr = upa_side_fr;
        data->short_dis_radar_bl = upa_side_rl;
        data->short_dis_radar_br = upa_side_rr;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    ApaUltrasonicLog::Type ApaUltrasonicLog::
                    LogType(const std::string& line_log) {
        if (line_log.find("apa") != std::string::npos)
            return kTypeAPA;
        else if (line_log.find("upa_mid") != std::string::npos)
            return kTypeUpaMid;
        else if (line_log.find("upa_side") != std::string::npos)
            return kTypeUpaSide;
        else
            return kTypeNone;
    }

    void ApaUltrasonicLog::MergeLog() {
        while (m_ultra_long_info.size()) {
            UltrasonicRadarInfo radar;
            radar.tm_ms = m_ultra_long_info.front().tm;
            radar.long_dis_radar_fl =
                m_ultra_long_info.front().long_dis_radar_fl;
            radar.long_dis_radar_fr =
                m_ultra_long_info.front().long_dis_radar_fr;
            radar.long_dis_radar_bl =
                m_ultra_long_info.front().long_dis_radar_bl;
            radar.long_dis_radar_br =
                m_ultra_long_info.front().long_dis_radar_br;
            m_ultra_long_info.pop_front();

            radar.short_dis_radar_flm =
                m_ultra_short_mid_info.front().short_dis_radar_flm;
            radar.short_dis_radar_frm =
                m_ultra_short_mid_info.front().short_dis_radar_frm;
            radar.short_dis_radar_blm =
                m_ultra_short_mid_info.front().short_dis_radar_blm;
            radar.short_dis_radar_brm =
                m_ultra_short_mid_info.front().short_dis_radar_brm;
            m_ultra_short_mid_info.pop_front();

            radar.short_dis_radar_fl =
                m_ultra_short_side_info.front().short_dis_radar_fl;
            radar.short_dis_radar_fr =
                m_ultra_short_side_info.front().short_dis_radar_fr;
            radar.short_dis_radar_bl =
                m_ultra_short_side_info.front().short_dis_radar_bl;
            radar.short_dis_radar_br =
                m_ultra_short_side_info.front().short_dis_radar_br;
            m_ultra_short_side_info.pop_front();

            m_radar_infos.push_back(radar);
        }
#if defined(OPEN_DEBUG)
        m_log_ofs.close();
#endif
    }

}  // namespace sim_log_reader
