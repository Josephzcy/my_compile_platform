// Copyright [2019] - MiniEye INC.
#pragma once
#include <string>
#include <fstream>
#include <deque>
#include <vector>
#include "apa_interface.h"

// DECLARE_string(apa_motion_log_addr);
namespace log_reader {

struct WheelSpeed {
    uint64_t tm;
    float spd_fl;
    float spd_fr;
    float spd_rl;
    float spd_rr;

    WheelSpeed():tm(0), spd_fl(0), spd_fr(0), spd_rl(0), spd_rr(0) {}

    WheelSpeed(const WheelSpeed& lhs) { *this = lhs; }

    const WheelSpeed& operator=(const WheelSpeed& lhs) {
        tm = lhs.tm;
        spd_fl = lhs.spd_fl;
        spd_fr = lhs.spd_fr;
        spd_rl = lhs.spd_rl;
        spd_rr = lhs.spd_rr;
        return *this;
    }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "tm: " << tm << std::endl;
        oss << "spd_fl: " << spd_fl << std::endl;
        oss << "spd_fr: " << spd_fr << std::endl;
        oss << "spd_rl: " << spd_rl << std::endl;
        oss << "spd_rr: " << spd_rr << std::endl;
        return oss.str();
    }
};

struct WheelPulseCounter {
    uint64_t tm;
    uint16_t counter_fl;
    uint16_t counter_fr;
    uint16_t counter_rl;
    uint16_t counter_rr;

    WheelPulseCounter():tm(0), counter_fl(0), counter_fr(0), counter_rl(0), counter_rr(0) {}

    WheelPulseCounter(const WheelPulseCounter& lhs) { *this = lhs; }

    const WheelPulseCounter& operator=(const WheelPulseCounter& lhs) {
        tm = lhs.tm;
        counter_fl = lhs.counter_fl;
        counter_fr = lhs.counter_fr;
        counter_rl = lhs.counter_rl;
        counter_rr = lhs.counter_rr;
        return *this;
    }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "tm: " << tm << std::endl;
        oss << "counter_fl: " << counter_fl << std::endl;
        oss << "counter_fr: " << counter_fr << std::endl;
        oss << "counter_rl: " << counter_rl << std::endl;
        oss << "counter_rr: " << counter_rr << std::endl;
        return oss.str();
    }
};

struct VehicleSpeed {
    uint64_t tm;
    float spd;

    VehicleSpeed():tm(0), spd(0) {}

    VehicleSpeed(const VehicleSpeed& lhs) {*this = lhs;}

    const VehicleSpeed& operator=(const VehicleSpeed& lhs) {
        tm = lhs.tm;
        spd = lhs.spd;
        return *this;
    }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "tm: " << tm << std::endl;
        oss << "spd: " << spd << std::endl;
        return oss.str();
    }
};

struct SteerAngle {
    uint64_t tm;
    float steer;

    SteerAngle():tm(0), steer(0) {}

    SteerAngle(const SteerAngle& lhs) {*this = lhs;}

    const SteerAngle& operator=(const SteerAngle& lhs) {
        tm = lhs.tm;
        steer = lhs.steer;
        return *this;
    }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "tm: " << tm << std::endl;
        oss << "steer: " << steer << std::endl;
        return oss.str();
    }
};

struct ShiftMode {
    uint64_t tm;
    int shift_mode;

    ShiftMode():tm(0), shift_mode(0) {}

    ShiftMode(const ShiftMode& lhs) {*this = lhs;}

    const ShiftMode& operator=(const ShiftMode& lhs) {
        tm = lhs.tm;
        shift_mode = lhs.shift_mode;
        return *this;
    }
};

struct UltraShortRadarInfo{
    uint64_t tm;
    double short_dis_radar_fl;
    double short_dis_radar_flm;
    double short_dis_radar_frm;
    double short_dis_radar_fr;
    double short_dis_radar_bl;
    double short_dis_radar_blm;
    double short_dis_radar_brm;
    double short_dis_radar_br;

    UltraShortRadarInfo()
        : tm(0),
        short_dis_radar_fl(-1),
        short_dis_radar_flm(-1),
        short_dis_radar_frm(-1),
        short_dis_radar_fr(-1),
        short_dis_radar_bl(-1),
        short_dis_radar_blm(-1),
        short_dis_radar_brm(-1),
        short_dis_radar_br(-1) {}

    UltraShortRadarInfo(const UltraShortRadarInfo& lhs) { *this = lhs; }

    const UltraShortRadarInfo& operator=(const UltraShortRadarInfo& lhs) {
        tm = lhs.tm;
        short_dis_radar_fl = lhs.short_dis_radar_fl;
        short_dis_radar_flm = lhs.short_dis_radar_flm;
        short_dis_radar_frm = lhs.short_dis_radar_frm;
        short_dis_radar_fr = lhs.short_dis_radar_fr;
        short_dis_radar_bl = lhs.short_dis_radar_bl;
        short_dis_radar_blm = lhs.short_dis_radar_blm;
        short_dis_radar_brm = lhs.short_dis_radar_brm;
        short_dis_radar_br = lhs.short_dis_radar_br;
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

class ApaMotionLog {
 public:
    ApaMotionLog();
    virtual ~ApaMotionLog();
    bool Init(const std::string& log_file, uint8_t ds_cal_mode);
    bool ReadMotionData(const uint64_t& timestamp, std::deque<TrackReqAuxInfo>* motion_data,
                                                    std::deque<UltrasonicRadarInfo>* radar_data);

 private:
    enum Type {
        kTypeSpeed = 0,
        kTypeWheelSpd,
        kTypeSteerAngle,
        kTypeShiftMode,
        kTypeWheelCounter,
        kTypeUltrasonicShortInfo,
        kTypeUltrasonicLongInfo,
        kTypeNone
    };
    enum Wheel {
        kFL = 0,
        kFR,
        kRL,
        kRR,
        kError
    };
    enum UltraNo {
        kShortRadarFL = 0,
        kShortRadarFLM,
        kShortRadarFRM,
        kShortRadarFR,
        kShortRadarBL,
        kShortRadarBLM,
        kShortRadarBRM,
        kShortRadarBR,
        kLongRadarFL,
        kLongRadarFR,
        kLongRadarBL,
        kLongRadarBR,
        kRadarError
    };

    bool ParseLog();
    void ParseSpeedLog(const std::string& log, VehicleSpeed* data);
    ApaMotionLog::Wheel ParseWheelSpdLog(const std::string& log, uint64_t* tm, float* data);
    ApaMotionLog::Wheel ParseWheelCounterLog(const std::string& log, uint64_t* tm, uint16_t* data);
    ApaMotionLog::UltraNo ParseUltraShortInfoLog(const std::string& log, uint64_t* tm, double* data);
    ApaMotionLog::UltraNo ParseUltraLongInfoLog(const std::string& log, uint64_t* tm, double* data);
    void ParseSteerAngleLog(const std::string& log, SteerAngle* data);
    void ParseShiftModeLog(const std::string& log, ShiftMode* data);
    ApaMotionLog::Type LogType(const std::string& line_log);
    void MergeLog();
    void ReadJsonLog(const std::string& jsonfile,
                    std::vector<CanInfo>* result,
                    uint8_t ds_cal_mode,
                    std::deque<TrackReqAuxInfo>* aux_list,
                    std::deque<UltrasonicRadarInfo>* radar_list);

 private:
    std::ifstream m_log_ifs;
    std::deque<WheelSpeed> m_wheel_spd;
    std::deque<WheelPulseCounter> m_wheel_counter;
    std::deque<UltraShortRadarInfo> m_ultra_short_info;
    std::deque<UltraLongRadarInfo> m_ultra_long_info;
    std::deque<VehicleSpeed> m_vehicle_spd;
    std::deque<SteerAngle> m_steer;
    std::deque<ShiftMode> m_shift_mode;
    std::deque<TrackReqAuxInfo> m_aux_infos;
    std::deque<UltrasonicRadarInfo> m_radar_infos;
    std::vector<CanInfo> m_can_results;
    uint64_t m_pre_img_tm;
    std::ofstream m_log_ofs;
};

}  // namespace log_reader
