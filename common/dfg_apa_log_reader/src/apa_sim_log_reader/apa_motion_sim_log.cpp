// Copyright [2019] - MiniEye INC.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "apa_motion_sim_log.h"

namespace sim_log_reader {

    ApaMotionLog::ApaMotionLog() {}

    ApaMotionLog::~ApaMotionLog() {}

    bool ApaMotionLog::Init(const std::string& log_file) {
        m_shift_mode.clear();
        m_steer.clear();
        m_vehicle_spd.clear();
        m_wheel_spd.clear();
        m_wheel_counter.clear();
        m_aux_infos.clear();
        m_veh_loc.clear();
        m_obj_loc_a.clear();
        m_obj_loc_b.clear();
        m_obj_loc_c.clear();
        m_bench_mark_infos.clear();
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

    bool ApaMotionLog::ReadMotionData(const uint64_t& timestamp,
                                      std::deque<TrackReqAuxInfo>* motion_data,
                                      std::deque<BenchMark>* benchmark_data) {
        motion_data->clear();
        benchmark_data->clear();
        if (!m_pre_img_tm) {
            TrackReqAuxInfo aux;
            BenchMark bm;
            while (m_aux_infos.size() &&
                    m_aux_infos.front().tm_ms < timestamp) {
                aux = m_aux_infos.front();
                bm = m_bench_mark_infos.front();
                m_aux_infos.pop_front();
                m_bench_mark_infos.pop_front();
            }
            motion_data->push_back(aux);
            benchmark_data->push_back(bm);
        } else {
            while (m_aux_infos.size() &&
                    m_aux_infos.front().tm_ms <= timestamp) {
                motion_data->push_back(m_aux_infos.front());
                benchmark_data->push_back(m_bench_mark_infos.front());
                m_aux_infos.pop_front();
                m_bench_mark_infos.pop_front();
            }
        }
        m_pre_img_tm = timestamp;
        if (motion_data->size()) {
            return true;
        } else {
            return false;
        }
    }

    bool ApaMotionLog::ParseLog() {
        if (!m_log_ifs.good()) {
            return false;
        }
        m_obj_loc_a.clear();
        m_obj_loc_b.clear();
        m_obj_loc_c.clear();
        std::string line_log = "";
        while (std::getline(m_log_ifs, line_log)) {
            VehicleSpeed vehicle_spd;
            SteerAngle steer_rad;
            ShiftMode shift_mode;
            WheelSpeed wheel_spd;
            WheelPulseCounter wheel_counter;
            VehicleLocation veh_loc;
            ObjectLocation obj_loc_a;   //  ObjectCar1
            ObjectLocation obj_loc_b;   //  ObjectCar2
            ObjectLocation obj_loc_c;   //  ObjectObstacle
            Type type = LogType(line_log);
            // std::cout << "raw log: " << line_log << std::endl;
            switch (type) {
                case kTypeSpeed:
                    ParseSpeedLog(line_log, &vehicle_spd);
                    m_vehicle_spd.push_back(vehicle_spd);
                    // std::cout << vehicle_spd.ToString();
                    // getchar();
                    break;
                case kTypeWheelSpd:
                    ParseWheelSpdLog(line_log, &wheel_spd);
                    m_wheel_spd.push_back(wheel_spd);
                    break;
                case kTypeWheelCounter:
                    ParseWheelCounterLog(line_log, &wheel_counter);
                    m_wheel_counter.push_back(wheel_counter);
                    break;
                case kTypeSteerAngle:
                    ParseSteerAngleLog(line_log, &steer_rad);
                    m_steer.push_back(steer_rad);
                    // std::cout << steer_rad.ToString();
                    // getchar();
                    break;
                case kTypeShiftMode:
                    ParseShiftModeLog(line_log, &shift_mode);
                    m_shift_mode.push_back(shift_mode);
                    break;
                case kTypeVehicleLocation:
                    ParseVehicleLocationLog(line_log, &veh_loc);
                    m_veh_loc.push_back(veh_loc);
                    break;
                case kTypeObjectLocationA:
                    ParseObjectLocationALog(line_log, &obj_loc_a);
                    m_obj_loc_a.push_back(obj_loc_a);
                    break;
                case kTypeObjectLocationB:
                    ParseObjectLocationBLog(line_log, &obj_loc_b);
                    m_obj_loc_b.push_back(obj_loc_b);
                    break;
                case kTypeObjectLocationC:
                    ParseObjectLocationCLog(line_log, &obj_loc_c);
                    m_obj_loc_c.push_back(obj_loc_c);
                    break;
                default:
                    break;
            }
        }
        return true;
    }

    void ApaMotionLog::ParseSpeedLog(const std::string& log,
                                    VehicleSpeed* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float speed_kmh = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp >> speed_kmh;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->spd = speed_kmh;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    void ApaMotionLog::ParseWheelSpdLog(const std::string& log,
                                        WheelSpeed* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float wheel_spd_fl = 0;
        float wheel_spd_fr = 0;
        float wheel_spd_rl = 0;
        float wheel_spd_rr = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp >> wheel_spd_fl
                >> wheel_spd_fr >> wheel_spd_rl >> wheel_spd_rr;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->spd_fl = wheel_spd_fl;
        data->spd_fr = wheel_spd_fr;
        data->spd_rl = wheel_spd_rl;
        data->spd_rr = wheel_spd_rr;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    void ApaMotionLog::ParseWheelCounterLog(const std::string& log,
                                            WheelPulseCounter* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float wheel_cnt_fl = 0;
        float wheel_cnt_fr = 0;
        float wheel_cnt_rl = 0;
        float wheel_cnt_rr = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp >> wheel_cnt_fl
                >> wheel_cnt_fr >> wheel_cnt_rl >> wheel_cnt_rr;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->counter_fl = wheel_cnt_fl;
        data->counter_fr = wheel_cnt_fr;
        data->counter_rl = wheel_cnt_rl;
        data->counter_rr = wheel_cnt_rr;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    void ApaMotionLog::ParseSteerAngleLog(const std::string& log,
                                            SteerAngle* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float steer_deg = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp >> steer_deg;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->steer = steer_deg;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    void ApaMotionLog::ParseShiftModeLog(const std::string& log,
                                            ShiftMode* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        int shift_mode = 0;
        std::string flag = "";
        ss_log >> seconds >> flag >> flag >> flag >> flag >> shift_mode;
        data->tm = static_cast<uint64_t>(seconds*1000);
        data->shift_mode = shift_mode;
    }
    void ApaMotionLog::ParseVehicleLocationLog(const std::string& log,
                                                VehicleLocation* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float x = 0;
        float y = 0;
        float z = 0;
        float roll = 0;
        float pitch = 0;
        float yaw = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp
        >> x >> y >> z >> roll >> pitch >> yaw;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->x = x;
        data->y = y;
        data->z = z;
        data->roll = roll;
        data->pitch = pitch;
        data->yaw = yaw;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }


    void ApaMotionLog::ParseObjectLocationALog(const std::string& log,
                                                ObjectLocation* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float x_world = 0;
        float y_world = 0;
        float z_world = 0;
        float x_relation = 0;
        float y_relation = 0;
        float z_relation = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp
        >> x_world >> y_world >> z_world
        >> x_relation >> y_relation >> z_relation;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->x_world = x_world;
        data->y_world = y_world;
        data->z_world = z_world;
        data->x_relation = x_relation;
        data->y_relation = y_relation;
        data->z_relation = z_relation;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }
    void ApaMotionLog::ParseObjectLocationBLog(const std::string& log,
                                                ObjectLocation* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float x_world = 0;
        float y_world = 0;
        float z_world = 0;
        float x_relation = 0;
        float y_relation = 0;
        float z_relation = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp
        >> x_world >> y_world >> z_world
        >> x_relation >> y_relation >> z_relation;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->x_world = x_world;
        data->y_world = y_world;
        data->z_world = z_world;
        data->x_relation = x_relation;
        data->y_relation = y_relation;
        data->z_relation = z_relation;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }
    void ApaMotionLog::ParseObjectLocationCLog(const std::string& log,
                                                ObjectLocation* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float x_world = 0;
        float y_world = 0;
        float z_world = 0;
        float x_relation = 0;
        float y_relation = 0;
        float z_relation = 0;
        std::string temp = "";
        std::string seconds_str = "";
        std::string seconds_s = "";
        std::string seconds_us = "";
        ss_log >> seconds_s >> seconds_us >> temp
        >> x_world >> y_world >> z_world
        >> x_relation >> y_relation >> z_relation;
        seconds_str = seconds_s + "." + seconds_us;
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        data->x_world = x_world;
        data->y_world = y_world;
        data->z_world = z_world;
        data->x_relation = x_relation;
        data->y_relation = y_relation;
        data->z_relation = z_relation;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    ApaMotionLog::Type ApaMotionLog::LogType(const std::string& line_log) {
        if (line_log.find("speed") != std::string::npos)
            return kTypeSpeed;
        else if (line_log.find("WheelSpeed") != std::string::npos)
            return kTypeWheelSpd;
        else if (line_log.find("SteeringAngle") != std::string::npos)
            return kTypeSteerAngle;
        else if (line_log.find("The actual shift mode") != std::string::npos)
            return kTypeShiftMode;
        else if (line_log.find("WheelCounter") != std::string::npos)
            return kTypeWheelCounter;
        else if (line_log.find("simloc") != std::string::npos)
            return kTypeVehicleLocation;
        else if (line_log.find("Obj_Vehicle_Buick3") != std::string::npos)
            return kTypeObjectLocationA;
        else if (line_log.find("Obj_Vehicle_Porsche3") != std::string::npos)
            return kTypeObjectLocationB;
        else if (line_log.find("Obj_SM_ConstCone_a") != std::string::npos)
            return kTypeObjectLocationC;
        else
            return kTypeNone;
    }

    void ApaMotionLog::MergeLog() {
        while (m_vehicle_spd.size()) {
            TrackReqAuxInfo aux;
            aux.shift_mode = 4;
            BenchMark bench_mark;
            // bool has_shift = false;
            bool has_steer = false;
            bool has_wheel_spd = false;
            bool has_wheel_cnt = false;
            aux.tm_ms = m_vehicle_spd.front().tm;
            aux.speed_kmph = m_vehicle_spd.front().spd;
            m_vehicle_spd.pop_front();
            while (m_steer.size() && m_steer.front().tm < aux.tm_ms) {
                m_steer.pop_front();
            }
            while (m_steer.size() && m_vehicle_spd.size() &&
                    m_steer.front().tm < m_vehicle_spd.front().tm) {
                aux.steer_deg = m_steer.front().steer;
                m_steer.pop_front();
                has_steer = true;
            }
            while (m_wheel_spd.size() && m_wheel_spd.front().tm < aux.tm_ms) {
                m_wheel_spd.pop_front();
            }
            while (m_wheel_spd.size() && m_vehicle_spd.size() &&
                    m_wheel_spd.front().tm < m_vehicle_spd.front().tm) {
                aux.wheel_spd_fl = m_wheel_spd.front().spd_fl;
                aux.wheel_spd_fr = m_wheel_spd.front().spd_fr;
                aux.wheel_spd_rl = m_wheel_spd.front().spd_rl;
                aux.wheel_spd_rr = m_wheel_spd.front().spd_rr;
                m_wheel_spd.pop_front();
                has_wheel_spd = true;
            }
            while (m_wheel_counter.size() && m_vehicle_spd.size() &&
                    m_wheel_counter.front().tm < m_vehicle_spd.front().tm) {
                aux.wheel_pulse_counter_fl = m_wheel_counter.front().counter_fl;
                aux.wheel_pulse_counter_fr = m_wheel_counter.front().counter_fr;
                aux.wheel_pulse_counter_rl = m_wheel_counter.front().counter_rl;
                aux.wheel_pulse_counter_rr = m_wheel_counter.front().counter_rr;
                m_wheel_counter.pop_front();
                has_wheel_cnt = true;
            }
            bench_mark.tm = m_veh_loc.front().tm;
            bench_mark.veh_loc = m_veh_loc.front();
            m_veh_loc.pop_front();
            if (m_obj_loc_a.size()) {
                bench_mark.obj_loc_a = m_obj_loc_a.front();
                m_obj_loc_a.pop_front();
            }
            if (m_obj_loc_b.size()) {
                bench_mark.obj_loc_b = m_obj_loc_b.front();
                m_obj_loc_b.pop_front();
            }

            if (m_obj_loc_c.size() > 0) {
                bench_mark.obj_loc_c = m_obj_loc_c.front();
                m_obj_loc_c.pop_front();
            }

            if (has_steer && has_wheel_spd && has_wheel_cnt) {
                m_aux_infos.push_back(aux);
                m_bench_mark_infos.push_back(bench_mark);
            }
        }
    }
}  // namespace sim_log_reader
