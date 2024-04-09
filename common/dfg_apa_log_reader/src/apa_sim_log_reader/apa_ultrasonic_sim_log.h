// Copyright [2019] - MiniEye INC.
#pragma once
#include <string>
#include <fstream>
#include <deque>
#include "apa_interface.h"

namespace sim_log_reader {
struct UltraShortRadarSideInfo {
    uint64_t tm;
    double short_dis_radar_fl;
    double short_dis_radar_fr;
    double short_dis_radar_bl;
    double short_dis_radar_br;

    UltraShortRadarSideInfo()
        : tm(0),
            short_dis_radar_fl(-1),
            short_dis_radar_fr(-1),
            short_dis_radar_bl(-1),
            short_dis_radar_br(-1) {}

    UltraShortRadarSideInfo(const UltraShortRadarSideInfo& lhs) { *this = lhs; }

    const UltraShortRadarSideInfo&
        operator=(const UltraShortRadarSideInfo& lhs) {
        tm = lhs.tm;
        short_dis_radar_fl = lhs.short_dis_radar_fl;
        short_dis_radar_fr = lhs.short_dis_radar_fr;
        short_dis_radar_bl = lhs.short_dis_radar_bl;
        short_dis_radar_br = lhs.short_dis_radar_br;
        return *this;
    }
};

struct UltraShortRadarMidInfo{
    uint64_t tm;
    double short_dis_radar_flm;
    double short_dis_radar_frm;
    double short_dis_radar_blm;
    double short_dis_radar_brm;

    UltraShortRadarMidInfo()
        : tm(0),
            short_dis_radar_flm(-1),
            short_dis_radar_frm(-1),
            short_dis_radar_blm(-1),
            short_dis_radar_brm(-1) {}

    UltraShortRadarMidInfo(const UltraShortRadarMidInfo& lhs) { *this = lhs; }

    const UltraShortRadarMidInfo& operator=(const UltraShortRadarMidInfo& lhs) {
        tm = lhs.tm;
        short_dis_radar_flm = lhs.short_dis_radar_flm;
        short_dis_radar_frm = lhs.short_dis_radar_frm;
        short_dis_radar_blm = lhs.short_dis_radar_blm;
        short_dis_radar_brm = lhs.short_dis_radar_brm;
        return *this;
    }
};

struct UltraLongRadarInfo{
    uint64_t tm;
    double long_dis_radar_fl;
    double long_dis_radar_fr;
    double long_dis_radar_bl;
    double long_dis_radar_br;

    UltraLongRadarInfo()
        : tm(0),
            long_dis_radar_fl(-1),
            long_dis_radar_fr(-1),
            long_dis_radar_bl(-1),
            long_dis_radar_br(-1) {}

    UltraLongRadarInfo(const UltraLongRadarInfo& lhs) { *this = lhs; }

    const UltraLongRadarInfo& operator=(const UltraLongRadarInfo& lhs) {
        tm = lhs.tm;
        long_dis_radar_fl = lhs.long_dis_radar_fl;
        long_dis_radar_fr = lhs.long_dis_radar_fr;
        long_dis_radar_bl = lhs.long_dis_radar_bl;
        long_dis_radar_br = lhs.long_dis_radar_br;
        return *this;
    }
};

class ApaUltrasonicLog {
 public:
    ApaUltrasonicLog();
    virtual ~ApaUltrasonicLog();
    bool Init(const std::string& log_file);
    bool ReadUltrasonicData(const uint64_t& timestamp,
                    std::deque<UltrasonicRadarInfo>* radar_data);

 private:
    enum Type {
        kTypeAPA = 0,
        kTypeUpaMid,
        kTypeUpaSide,
        kTypeNone
    };

    bool ParseLog();
    void ParseApaLog(const std::string& log, UltraLongRadarInfo* data);
    void ParseUpaMidLog(const std::string& log, UltraShortRadarMidInfo* data);
    void ParseUpaSideLog(const std::string& log, UltraShortRadarSideInfo* data);
    ApaUltrasonicLog::Type LogType(const std::string& line_log);
    void MergeLog();

 private:
    std::ifstream m_log_ifs;
    std::deque<UltraLongRadarInfo> m_ultra_long_info;
    std::deque<UltraShortRadarMidInfo> m_ultra_short_mid_info;
    std::deque<UltraShortRadarSideInfo> m_ultra_short_side_info;
    std::deque<UltrasonicRadarInfo> m_radar_infos;
    uint64_t m_pre_img_tm;
#if defined(OPEN_DEBUG)
    std::ofstream m_log_ofs;
#endif
};

}  // namespace sim_log_reader
