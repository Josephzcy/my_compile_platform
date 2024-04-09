// Copyright [2019] - MiniEye INC.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "apa_motion_log.h"
#include "json.hpp"

namespace log_reader {

    ApaMotionLog::ApaMotionLog() {
    }

    ApaMotionLog::~ApaMotionLog() {
    }

    bool ApaMotionLog::Init(const std::string& log_file, uint8_t ds_cal_mode) {
        m_shift_mode.clear();
        m_steer.clear();
        m_vehicle_spd.clear();
        m_wheel_spd.clear();
        m_wheel_counter.clear();
        m_ultra_short_info.clear();
        m_ultra_long_info.clear();
        m_aux_infos.clear();
        m_radar_infos.clear();
        m_pre_img_tm = 0;
        m_log_ifs.open(log_file.c_str(), std::ifstream::in);
        // m_log_ofs.open("motion.txt", std::ofstream::out);
        if (!m_log_ifs.good()) {
            std::cerr << "open log file failed: " << log_file << std::endl;
            return false;
        } else {
            std::cerr << "parse apa motion log file : " << log_file << std::endl;
            // bool ret = ParseLog();
            // MergeLog();
            // std::cout << "spd_size = " << m_vehicle_spd.size() << std::endl;
            // std::cout << "wheel_size = " << m_wheel_spd.size() << std::endl;
            // std::cout << "steer_size = " << m_steer.size() << std::endl;
            ReadJsonLog(log_file, &m_can_results, ds_cal_mode, &m_aux_infos, &m_radar_infos);
            return true;
        }
    }

    bool ApaMotionLog::ReadMotionData(const uint64_t& timestamp,
                                        std::deque<TrackReqAuxInfo>* motion_data,
                                        std::deque<UltrasonicRadarInfo>* radar_data) {
        motion_data->clear();
        radar_data->clear();
        if (!m_pre_img_tm) {
            TrackReqAuxInfo aux;
            UltrasonicRadarInfo radar_info;
            while (m_aux_infos.size() && m_aux_infos.front().tm_ms < timestamp) {
                aux = m_aux_infos.front();
                m_aux_infos.pop_front();
            }
            motion_data->push_back(aux);
            while (m_radar_infos.size() && m_radar_infos.front().tm_ms < timestamp) {
                radar_info = m_radar_infos.front();
                m_radar_infos.pop_front();
            }
            radar_data->push_back(radar_info);
        } else {
            while (m_aux_infos.size() && m_aux_infos.front().tm_ms < timestamp) {
                motion_data->push_back(m_aux_infos.front());
                m_aux_infos.pop_front();
            }
            while (m_radar_infos.size() && m_radar_infos.front().tm_ms < timestamp) {
                radar_data->push_back(m_radar_infos.front());
                m_radar_infos.pop_front();
            }
        }
        m_pre_img_tm = timestamp;
        if (motion_data->size() && radar_data->size()) {
            return true;
        } else {
            return false;
        }
    }

    bool ApaMotionLog::ParseLog() {
        if (!m_log_ifs.good()) {
            return false;
        }
        std::string line_log = "";
        WheelSpeed wheel_spd;
        WheelPulseCounter wheel_counter;
        UltraShortRadarInfo ultra_short_info;
        UltraLongRadarInfo ultra_long_info;
        int wheel_spd_cnt = 0;
        int wheel_counter_cnt = 0;
        int ultra_short_info_cnt = 0;
        int ultra_long_info_cnt = 0;
        while (std::getline(m_log_ifs, line_log)) {
            VehicleSpeed vehicle_spd;
            SteerAngle steer_rad;
            ShiftMode shift_mode;
            uint64_t tm_read = 0;
            Wheel wheel = kError;
            UltraNo ultra_no = kRadarError;
            float wheel_speed = 0;
            uint16_t wheel_cnt = 0;
            double ultrasonic_short_info = 0;
            double ultrasonic_long_info = 0;
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
                    wheel = ParseWheelSpdLog(line_log, &tm_read, &wheel_speed);
                    switch (wheel) {
                        case kFL:
                            if (wheel_spd.tm != tm_read) {
                                wheel_spd.tm = tm_read;
                                wheel_spd_cnt = 0;
                            } else {
                                wheel_spd_cnt++;
                            }
                            wheel_spd.spd_fl = wheel_speed;
                            break;
                        case kFR:
                            if (wheel_spd.tm != tm_read) {
                                wheel_spd.tm = tm_read;
                                wheel_spd_cnt = 0;
                            } else {
                                wheel_spd_cnt++;
                            }
                            wheel_spd.spd_fr = wheel_speed;
                            break;
                        case kRL:
                            if (wheel_spd.tm != tm_read) {
                                wheel_spd.tm = tm_read;
                                wheel_spd_cnt = 0;
                            } else {
                                wheel_spd_cnt++;
                            }
                            wheel_spd.spd_rl = wheel_speed;
                            break;
                        case kRR:
                            if (wheel_spd.tm != tm_read) {
                                wheel_spd.tm = tm_read;
                                wheel_spd_cnt = 0;
                            } else {
                                wheel_spd_cnt++;
                            }
                            wheel_spd.spd_rr = wheel_speed;
                            break;
                        default:
                            break;
                    }
                    // std::cout << "wheel_spd_cnt = " << wheel_spd_cnt << std::endl;
                    // getchar();
                    if (wheel_spd_cnt > 2) {
                        m_wheel_spd.push_back(wheel_spd);
                        // std::cout << wheel_spd.ToString();
                    }
                    break;
                case kTypeWheelCounter:
                    wheel = ParseWheelCounterLog(line_log, &tm_read, &wheel_cnt);
                    switch (wheel) {
                        case kFL:
                            if (wheel_counter.tm != tm_read) {
                                wheel_counter.tm = tm_read;
                                wheel_counter_cnt = 0;
                            } else {
                                wheel_counter_cnt++;
                            }
                            wheel_counter.counter_fl = wheel_cnt;
                            break;
                        case kFR:
                            if (wheel_counter.tm != tm_read) {
                                wheel_counter.tm = tm_read;
                                wheel_counter_cnt = 0;
                            } else {
                                wheel_counter_cnt++;
                            }
                            wheel_counter.counter_fr = wheel_cnt;
                            break;
                        case kRL:
                            if (wheel_counter.tm != tm_read) {
                                wheel_counter.tm = tm_read;
                                wheel_counter_cnt = 0;
                            } else {
                                wheel_counter_cnt++;
                            }
                            wheel_counter.counter_rl = wheel_cnt;
                            break;
                        case kRR:
                            if (wheel_counter.tm != tm_read) {
                                wheel_counter.tm = tm_read;
                                wheel_counter_cnt = 0;
                            } else {
                                wheel_counter_cnt++;
                            }
                            wheel_counter.counter_rr = wheel_cnt;
                            break;
                        default:
                            break;
                    }
                    // std::cout << "wheel_counter_cnt = " << wheel_counter_cnt << std::endl;
                    // getchar();
                    if (wheel_counter_cnt > 2) {
                        m_wheel_counter.push_back(wheel_counter);
                        // std::cout << wheel_spd.ToString();
                    }
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
                case kTypeUltrasonicShortInfo:
                    ultra_no = ParseUltraShortInfoLog(line_log, &tm_read, &ultrasonic_short_info);
                    switch (ultra_no) {
                        case kShortRadarFL:
                            if (ultra_short_info.tm != tm_read) {
                                ultra_short_info.tm = tm_read;
                                ultra_short_info_cnt = 0;
                            } else {
                                ultra_short_info_cnt++;
                            }
                            ultra_short_info.short_dis_radar_fl = ultrasonic_short_info;
                            break;
                        case kShortRadarFLM:
                            if (ultra_short_info.tm != tm_read) {
                                ultra_short_info.tm = tm_read;
                                ultra_short_info_cnt = 0;
                            } else {
                                ultra_short_info_cnt++;
                            }
                            ultra_short_info.short_dis_radar_flm = ultrasonic_short_info;
                            break;
                        case kShortRadarFRM:
                            if (ultra_short_info.tm != tm_read) {
                                ultra_short_info.tm = tm_read;
                                ultra_short_info_cnt = 0;
                            } else {
                                ultra_short_info_cnt++;
                            }
                            ultra_short_info.short_dis_radar_frm = ultrasonic_short_info;
                            break;
                        case kShortRadarFR:
                            if (ultra_short_info.tm != tm_read) {
                                ultra_short_info.tm = tm_read;
                                ultra_short_info_cnt = 0;
                            } else {
                                ultra_short_info_cnt++;
                            }
                            ultra_short_info.short_dis_radar_fr = ultrasonic_short_info;
                            break;
                        case kShortRadarBL:
                            if (ultra_short_info.tm != tm_read) {
                                ultra_short_info.tm = tm_read;
                                ultra_short_info_cnt = 0;
                            } else {
                                ultra_short_info_cnt++;
                            }
                            ultra_short_info.short_dis_radar_bl = ultrasonic_short_info;
                            break;
                        case kShortRadarBLM:
                            if (ultra_short_info.tm != tm_read) {
                                ultra_short_info.tm = tm_read;
                                ultra_short_info_cnt = 0;
                            } else {
                                ultra_short_info_cnt++;
                            }
                            ultra_short_info.short_dis_radar_blm = ultrasonic_short_info;
                            break;
                        case kShortRadarBRM:
                            if (ultra_short_info.tm != tm_read) {
                                ultra_short_info.tm = tm_read;
                                ultra_short_info_cnt = 0;
                            } else {
                                ultra_short_info_cnt++;
                            }
                            ultra_short_info.short_dis_radar_brm = ultrasonic_short_info;
                            break;
                        case kShortRadarBR:
                            if (ultra_short_info.tm != tm_read) {
                                ultra_short_info.tm = tm_read;
                                ultra_short_info_cnt = 0;
                            } else {
                                ultra_short_info_cnt++;
                            }
                            ultra_short_info.short_dis_radar_br = ultrasonic_short_info;
                            break;
                        default:
                            break;
                    }
                    // std::cout << "ultra_short_info_cnt = " << ultra_short_info_cnt << std::endl;
                    if (ultra_short_info_cnt > 6) {
                        m_ultra_short_info.push_back(ultra_short_info);
                        // std::cout << wheel_spd.ToString();
                    }
                    break;
                case kTypeUltrasonicLongInfo:
                    ultra_no = ParseUltraLongInfoLog(line_log, &tm_read, &ultrasonic_long_info);
                    switch (ultra_no) {
                        case kLongRadarFL:
                            if (ultra_long_info.tm != tm_read) {
                                ultra_long_info.tm = tm_read;
                                ultra_long_info_cnt = 0;
                            } else {
                                ultra_long_info_cnt++;
                            }
                            ultra_long_info.long_dis_radar_fl = ultrasonic_long_info;
                            break;
                        case kLongRadarFR:
                            if (ultra_long_info.tm != tm_read) {
                                ultra_long_info.tm = tm_read;
                                ultra_long_info_cnt = 0;
                            } else {
                                ultra_long_info_cnt++;
                            }
                            ultra_long_info.long_dis_radar_fr = ultrasonic_long_info;
                            break;
                        case kLongRadarBL:
                            if (ultra_long_info.tm != tm_read) {
                                ultra_long_info.tm = tm_read;
                                ultra_long_info_cnt = 0;
                            } else {
                                ultra_long_info_cnt++;
                            }
                            ultra_long_info.long_dis_radar_bl = ultrasonic_long_info;
                            break;
                        case kLongRadarBR:
                            if (ultra_long_info.tm != tm_read) {
                                ultra_long_info.tm = tm_read;
                                ultra_long_info_cnt = 0;
                            } else {
                                ultra_long_info_cnt++;
                            }
                            ultra_long_info.long_dis_radar_br = ultrasonic_long_info;
                            break;
                        default:
                            break;
                    }
                    // std::cout << "ultra_long_info_cnt = " << ultra_long_info_cnt << std::endl;
                    if (ultra_long_info_cnt > 2) {
                        m_ultra_long_info.push_back(ultra_long_info);
                        // std::cout << wheel_spd.ToString();
                    }
                    break;
                default:
                    break;
            }
        }
        return true;
    }

    void ApaMotionLog::ParseSpeedLog(const std::string& log, VehicleSpeed* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float speed_kmh = 0;
        std::string speed_flag = "";
        ss_log >> seconds >> speed_flag >> speed_flag >> speed_kmh;
        data->spd = speed_kmh;
        data->tm = static_cast<uint64_t>(seconds*1000);
    }

    ApaMotionLog::Wheel ApaMotionLog::ParseWheelSpdLog(const std::string& log, uint64_t* tm, float* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float speed_kmh = 0;
        std::string wheel_spd_flag = "";
        ss_log >> seconds >> wheel_spd_flag >> speed_kmh;
        *tm = static_cast<uint64_t>(seconds * 1000);
        *data = speed_kmh;
        if (wheel_spd_flag == "WheelSpeed_FL") {
            return kFL;
        } else if (wheel_spd_flag == "WheelSpeed_FR") {
            return kFR;
        } else if (wheel_spd_flag == "WheelSpeed_RL") {
            return kRL;
        } else if (wheel_spd_flag == "WheelSpeed_RR") {
            return kRR;
        } else {
            return kError;
        }
    }

    ApaMotionLog::Wheel ApaMotionLog::ParseWheelCounterLog(const std::string& log, uint64_t* tm, uint16_t* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        uint16_t wheel_counter = 0;
        std::string wheel_counter_flag = "";
        std::string counter_temp = "";
        ss_log >> seconds >> wheel_counter_flag >> counter_temp >>  counter_temp >> counter_temp >> wheel_counter;
        *tm = static_cast<uint64_t>(seconds * 1000);
        *data = wheel_counter;
        if (wheel_counter_flag == "FL") {
            return kFL;
        } else if (wheel_counter_flag == "FR") {
            return kFR;
        } else if (wheel_counter_flag == "RL") {
            return kRL;
        } else if (wheel_counter_flag == "RR") {
            return kRR;
        } else {
            return kError;
        }
    }

    ApaMotionLog::UltraNo ApaMotionLog::ParseUltraShortInfoLog(const std::string& log, uint64_t* tm, double* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        double ultra_range = 0;
        std::string ultra_range_flag = "";
        std::string counter_temp = "";
        ss_log >> seconds >> counter_temp >> counter_temp >>  counter_temp >> ultra_range_flag >> ultra_range;
        *tm = static_cast<uint64_t>(seconds * 1000);
        *data = ultra_range;
        if (ultra_range_flag == "fl") {
            return kShortRadarFL;
        } else if (ultra_range_flag == "flm") {
            return kShortRadarFLM;
        } else if (ultra_range_flag == "frm") {
            return kShortRadarFRM;
        } else if (ultra_range_flag == "fr") {
            return kShortRadarFR;
        } else if (ultra_range_flag == "bl") {
            return kShortRadarBL;
        } else if (ultra_range_flag == "blm") {
            return kShortRadarBLM;
        } else if (ultra_range_flag == "brm") {
            return kShortRadarBRM;
        } else if (ultra_range_flag == "br") {
            return kShortRadarBR;
        } else {
            return kRadarError;
        }
    }

    ApaMotionLog::UltraNo ApaMotionLog::ParseUltraLongInfoLog(const std::string& log, uint64_t* tm, double* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        double ultra_range = 0;
        std::string ultra_range_flag = "";
        std::string counter_temp = "";
        ss_log >> seconds >> counter_temp >> counter_temp >>  counter_temp >> ultra_range_flag >> ultra_range;
        *tm = static_cast<uint64_t>(seconds * 1000);
        *data = ultra_range;
        if (ultra_range_flag == "fl") {
            return kLongRadarFL;
        } else if (ultra_range_flag == "fr") {
            return kLongRadarFR;
        } else if (ultra_range_flag == "bl") {
            return kLongRadarBL;
        } else if (ultra_range_flag == "br") {
            return kLongRadarBR;
        } else {
            return kRadarError;
        }
    }

    void ApaMotionLog::ParseSteerAngleLog(const std::string& log, SteerAngle* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        float steer_deg = 0;
        std::string steer_flag = "";
        ss_log >> seconds >> steer_flag >> steer_deg;
        data->tm = static_cast<uint64_t>(seconds*1000);
        data->steer = steer_deg;
    }

    void ApaMotionLog::ParseShiftModeLog(const std::string& log, ShiftMode* data) {
        std::stringstream ss_log;
        ss_log.str(log);
        double seconds = 0;
        int shift_mode = 0;
        std::string flag = "";
        ss_log >> seconds >> flag >> flag >> flag >> flag >> shift_mode;
        data->tm = static_cast<uint64_t>(seconds*1000);
        data->shift_mode = shift_mode;
    }

    ApaMotionLog::Type ApaMotionLog::LogType(const std::string& line_log) {
        if (line_log.find("vehicle speed") != std::string::npos)
            return kTypeSpeed;
        else if (line_log.find("WheelSpeed") != std::string::npos)
            return kTypeWheelSpd;
        else if (line_log.find("angle") != std::string::npos)
            return kTypeSteerAngle;
        else if (line_log.find("The actual shift mode") != std::string::npos)
            return kTypeShiftMode;
        else if (line_log.find("Wheel Pulse Counter") != std::string::npos)
            return kTypeWheelCounter;
        else if (line_log.find("short dis radar") != std::string::npos)
            return kTypeUltrasonicShortInfo;
        else if (line_log.find("long dis radar") != std::string::npos)
            return kTypeUltrasonicLongInfo;
        else
            return kTypeNone;
    }

    void ApaMotionLog::MergeLog() {
        while (m_vehicle_spd.size()) {
            TrackReqAuxInfo aux;
            UltrasonicRadarInfo radar;
            bool has_shift = false;
            bool has_steer = false;
            bool has_wheel = false;
            bool has_wheel_cnt = false;
            bool has_ultra_short_info = false;
            bool has_ultra_long_info = false;
            aux.tm_ms = m_vehicle_spd.front().tm;
            radar.tm_ms = m_vehicle_spd.front().tm;
            aux.speed_kmph = m_vehicle_spd.front().spd;
            m_vehicle_spd.pop_front();
            while (m_shift_mode.size() && m_shift_mode.front().tm < aux.tm_ms) {
                aux.shift_mode = m_shift_mode.front().shift_mode;
                m_shift_mode.pop_front();
                has_shift = true;
            }
            while (!has_shift && m_shift_mode.size() && m_vehicle_spd.size()
                   && m_shift_mode.front().tm < m_vehicle_spd.front().tm) {
                aux.shift_mode = m_shift_mode.front().shift_mode;
                has_shift = true;
            }

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
                has_wheel = true;
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
            while (m_ultra_short_info.size() && m_vehicle_spd.size() &&
                    m_ultra_short_info.front().tm < m_vehicle_spd.front().tm) {
                radar.short_dis_radar_fl = m_ultra_short_info.front().short_dis_radar_fl;
                radar.short_dis_radar_flm = m_ultra_short_info.front().short_dis_radar_flm;
                radar.short_dis_radar_frm = m_ultra_short_info.front().short_dis_radar_frm;
                radar.short_dis_radar_fr = m_ultra_short_info.front().short_dis_radar_fr;
                radar.short_dis_radar_bl = m_ultra_short_info.front().short_dis_radar_bl;
                radar.short_dis_radar_blm = m_ultra_short_info.front().short_dis_radar_blm;
                radar.short_dis_radar_brm = m_ultra_short_info.front().short_dis_radar_brm;
                radar.short_dis_radar_br = m_ultra_short_info.front().short_dis_radar_br;
                m_ultra_short_info.pop_front();
                has_ultra_short_info = true;
            }
            while (m_ultra_long_info.size() && m_vehicle_spd.size() &&
                    m_ultra_long_info.front().tm < m_vehicle_spd.front().tm) {
                radar.long_dis_radar_fl = m_ultra_long_info.front().long_dis_radar_fl;
                radar.long_dis_radar_fr = m_ultra_long_info.front().long_dis_radar_fr;
                radar.long_dis_radar_bl = m_ultra_long_info.front().long_dis_radar_bl;
                radar.long_dis_radar_br = m_ultra_long_info.front().long_dis_radar_br;
                m_ultra_long_info.pop_front();
                has_ultra_long_info = true;
            }
            if(has_ultra_short_info && has_ultra_long_info){
                m_radar_infos.push_back(radar);
            }
            if (has_shift && has_steer && has_wheel /*&& has_wheel_cnt*/) {
                m_aux_infos.push_back(aux);
                char buf[2000];
                snprintf(buf, sizeof(buf), "{\"speed\":%.5f, \"ts\":%lu, \"turnlump\": %d, \"canValue\" : [0.0, %.1f, %f, %f, %f, %f, %f, %f, %f, %f, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f]}\n",
                    aux.speed_kmph / 3.6, aux.tm_ms * 1000, 0, aux.steer_deg, 0.0, 0.0, 0.0, 0.0,
                    aux.wheel_spd_fl / 3.6, aux.wheel_spd_fr / 3.6, aux.wheel_spd_rl / 3.6, aux.wheel_spd_rr / 3.6, aux.shift_mode,
                    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
                m_log_ofs << buf;
            }
        }
    }

    void ApaMotionLog::ReadJsonLog(const std::string& jsonfile,
                        std::vector<CanInfo>* result,
                        uint8_t ds_cal_mode,
                        std::deque<TrackReqAuxInfo>* aux_list,
                        std::deque<UltrasonicRadarInfo>* radar_list) {
        result->clear();
        aux_list->clear();
        radar_list->clear();
        std::string jsonstring = "";
        std::ifstream in(jsonfile);
        while (std::getline(in, jsonstring)) {
            nlohmann::json config_json;
            CanInfo m_can_info;
            TrackReqAuxInfo aux;
            UltrasonicRadarInfo radar;
            double seconds = 0;
            config_json = nlohmann::json::parse(jsonstring);
            m_can_info.canValue =
                config_json.at("canValue").get<std::vector<float> >();
            m_can_info.speed = config_json.at("speed").get<float>();
            m_can_info.ts = config_json.at("ts").get<uint64_t>();
            // m_can_info.ts /= 1000;
            m_can_info.turnlump = config_json.at("turnlump").get<int>();
            result->push_back(m_can_info);
            std::string seconds_str = std::to_string(m_can_info.ts);
            std::string temp = ".";
            seconds_str.insert(10, temp);
            std::istringstream ss_second(seconds_str);
            ss_second >> seconds;
            aux.tm_ms = static_cast<uint64_t>(seconds*1000);
            aux.ds_cal_mode = ds_cal_mode;
            // aux.tm_ms = m_can_info.ts;
            aux.speed_kmph = m_can_info.speed;
            aux.speed_kmph = 0.5*(m_can_info.canValue[28] + m_can_info.canValue[29]);
            aux.steer_deg = m_can_info.canValue[1];
            int direction = static_cast<int>(m_can_info.canValue[2]);
            if (direction == 1) {
                aux.steer_deg *= -1;
            }
            aux.wheel_pulse_counter_fl = m_can_info.canValue[5];
            aux.wheel_pulse_counter_fr = m_can_info.canValue[7];
            aux.wheel_pulse_counter_rl = m_can_info.canValue[9];
            aux.wheel_pulse_counter_rr = m_can_info.canValue[11];
            aux.wheel_spd_fl = m_can_info.canValue[26];
            aux.wheel_spd_fr = m_can_info.canValue[27];
            aux.wheel_spd_rl = m_can_info.canValue[28];
            aux.wheel_spd_rr = m_can_info.canValue[29];

            aux.shift_mode = m_can_info.canValue[13];
            aux.driver_torque = (1.0 - m_can_info.canValue[4]) * m_can_info.canValue[3];
            aux.longitudinal_acceleration = m_can_info.canValue[32];
            aux.lateral_acceleration = m_can_info.canValue[33];
            if (m_can_info.canValue[31] == 1) {
                aux.yaw_rate = m_can_info.canValue[30];
            } else {
                aux.yaw_rate = -m_can_info.canValue[30];
            }
            aux_list->push_back(aux);
            radar.tm_ms = static_cast<uint64_t>(seconds*1000);
            radar.short_dis_radar_fl = m_can_info.canValue[14] * 10.0;
            radar.short_dis_radar_flm = m_can_info.canValue[15] * 10.0;
            radar.short_dis_radar_frm = m_can_info.canValue[16] * 10.0;
            radar.short_dis_radar_fr = m_can_info.canValue[17] * 10.0;
            radar.short_dis_radar_bl = m_can_info.canValue[18] * 10.0;
            radar.short_dis_radar_blm = m_can_info.canValue[19] * 10.0;
            radar.short_dis_radar_brm = m_can_info.canValue[20] * 10.0;
            radar.short_dis_radar_br = m_can_info.canValue[21] * 10.0;
            radar.long_dis_radar_fl = m_can_info.canValue[23] * 10.0;
            radar.long_dis_radar_fr = m_can_info.canValue[22] * 10.0;
            radar.long_dis_radar_bl = m_can_info.canValue[24] * 10.0;
            radar.long_dis_radar_br = m_can_info.canValue[25] * 10.0;
            radar_list->push_back(radar);
        }
    }
}  // namespace log_reader
