// Copyright [2019] - MiniEye INC.
#pragma once
#include <string>
#include <fstream>
#include <deque>
#include "apa_interface.h"

// DECLARE_string(apa_motion_log_addr);
namespace sim_log_reader {

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

    WheelPulseCounter():tm(0),
        counter_fl(0), counter_fr(0), counter_rl(0), counter_rr(0) {}

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

struct VehicleLocation {
    uint64_t tm;
    float x;
    float y;
    float z;
    float roll;
    float pitch;
    float yaw;
    /*  车辆坐标基于UE4世界坐标系，默认情况下x向前y向右z向上，
        UE4使用左手坐标系。坐标单位m，角度单位 */
    VehicleLocation():tm(0), x(0), y(0), z(0),
                    roll(0), pitch(0), yaw(0) {}

    VehicleLocation(const VehicleLocation& lhs) {*this = lhs;}

    const VehicleLocation& operator=(const VehicleLocation& lhs) {
        tm = lhs.tm;
        x = lhs.x;
        y = lhs.y;
        z = lhs.z;
        roll = lhs.roll;
        pitch = lhs.pitch;
        yaw = lhs.yaw;
        return *this;
    }
};

struct ObjectLocation {
    uint64_t tm;
    float x_world;
    float y_world;
    float z_world;
    float x_relation;
    float y_relation;
    float z_relation;
    /*表示物体的名称。相对坐标是基于车辆中心的坐标系，x轴为车辆前进方向，
        y轴为垂直前进往右的方向，z轴为垂直海平面向上。坐标单位m */
    ObjectLocation():tm(0), x_world(0), y_world(0), z_world(0),
                 x_relation(0), y_relation(0), z_relation(0) {}

    ObjectLocation(const ObjectLocation& lhs) {*this = lhs;}

    const ObjectLocation& operator=(const ObjectLocation& lhs) {
        tm = lhs.tm;
        x_world = lhs.x_world;
        y_world = lhs.y_world;
        z_world = lhs.z_world;
        x_relation = lhs.x_relation;
        y_relation = lhs.y_relation;
        z_relation = lhs.z_relation;
        return *this;
    }
};
struct BenchMark {
    uint64_t tm;
    VehicleLocation veh_loc;
    ObjectLocation obj_loc_a;   //  ObjectCar1
    ObjectLocation obj_loc_b;   //  ObjectCar2
    ObjectLocation obj_loc_c;   //  ObjectObstacle

    BenchMark() {}

    BenchMark(const BenchMark& lhs) {*this = lhs;}

    const BenchMark& operator=(const BenchMark& lhs) {
        tm = lhs.tm;
        veh_loc = lhs.veh_loc;
        obj_loc_a = lhs.obj_loc_a;
        obj_loc_b = lhs.obj_loc_b;
        obj_loc_c = lhs.obj_loc_c;
        return *this;
    }
};

class ApaMotionLog {
 public:
    ApaMotionLog();
    virtual ~ApaMotionLog();
    bool Init(const std::string& log_file);
    bool ReadMotionData(const uint64_t& timestamp,
                        std::deque<TrackReqAuxInfo>* motion_data,
                        std::deque<BenchMark>* benchmark_data);

 private:
    enum Type {
        kTypeSpeed = 0,
        kTypeWheelSpd,
        kTypeSteerAngle,
        kTypeShiftMode,
        kTypeWheelCounter,
        kTypeVehicleLocation,
        kTypeObjectLocationA,
        kTypeObjectLocationB,
        kTypeObjectLocationC,
        kTypeNone
    };
    enum Wheel {
        kFL = 0,
        kFR,
        kRL,
        kRR,
        kError
    };

    bool ParseLog();
    void ParseSpeedLog(const std::string& log, VehicleSpeed* data);
    void ParseWheelSpdLog(const std::string& log, WheelSpeed* data);
    void ParseWheelCounterLog(const std::string& log, WheelPulseCounter* data);
    void ParseSteerAngleLog(const std::string& log, SteerAngle* data);
    void ParseShiftModeLog(const std::string& log, ShiftMode* data);
    void ParseVehicleLocationLog(const std::string& log, VehicleLocation* data);
    void ParseObjectLocationALog(const std::string& log, ObjectLocation* data);
    void ParseObjectLocationBLog(const std::string& log, ObjectLocation* data);
    void ParseObjectLocationCLog(const std::string& log, ObjectLocation* data);
    ApaMotionLog::Type LogType(const std::string& line_log);
    void MergeLog();

 private:
    std::ifstream m_log_ifs;
    std::deque<WheelSpeed> m_wheel_spd;
    std::deque<WheelPulseCounter> m_wheel_counter;
    std::deque<VehicleSpeed> m_vehicle_spd;
    std::deque<SteerAngle> m_steer;
    std::deque<ShiftMode> m_shift_mode;
    std::deque<VehicleLocation> m_veh_loc;
    std::deque<ObjectLocation> m_obj_loc_a;
    std::deque<ObjectLocation> m_obj_loc_b;
    std::deque<ObjectLocation> m_obj_loc_c;
    std::deque<TrackReqAuxInfo> m_aux_infos;
    std::deque<BenchMark> m_bench_mark_infos;
    uint64_t m_pre_img_tm;
};
}  // namespace sim_log_reader
