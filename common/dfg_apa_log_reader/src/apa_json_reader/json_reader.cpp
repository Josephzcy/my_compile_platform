// Copyright [2019] - MiniEye INC.
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include "json.hpp"
#include "opencv2/opencv.hpp"
#include "msgpack.hpp"
//#include "common/concurrency/rwlock.h"
#include "common/concurrency/work_thread.h"
#include "common/time/time_utils.h"
#include "common/concurrency/this_thread.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "apa_interface.h"
#include "json_reader.h"

void ReadJsonLog(const std::string& jsonfile,
                    std::deque<CanInfo>* result,
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
        m_can_info.turnlump = config_json.at("turnlump").get<int>();
        result->push_back(m_can_info);
        std::string seconds_str = std::to_string(m_can_info.ts);
        std::string temp = ".";
        seconds_str.insert(10, temp);
        std::istringstream ss_second(seconds_str);
        ss_second >> seconds;
        aux.tm_ms = static_cast<uint64_t>(seconds*1000);
        aux.speed_kmph = m_can_info.speed;
        aux.steer_deg = m_can_info.canValue[1];
        aux.wheel_spd_fl = m_can_info.canValue[6];
        aux.wheel_spd_fr = m_can_info.canValue[7];
        aux.wheel_spd_rl = m_can_info.canValue[8];
        aux.wheel_spd_rr = m_can_info.canValue[9];
        aux.shift_mode = m_can_info.canValue[10];
        aux_list->push_back(aux);
        radar.tm_ms = static_cast<uint64_t>(seconds*1000);
        radar.short_dis_radar_fl = m_can_info.canValue[12] * 10.0;
        radar.short_dis_radar_flm = m_can_info.canValue[13] * 10.0;
        radar.short_dis_radar_frm = m_can_info.canValue[14] * 10.0;
        radar.short_dis_radar_fr = m_can_info.canValue[15] * 10.0;
        radar.short_dis_radar_bl = m_can_info.canValue[16] * 10.0;
        radar.short_dis_radar_blm = m_can_info.canValue[17] * 10.0;
        radar.short_dis_radar_brm = m_can_info.canValue[18] * 10.0;
        radar.short_dis_radar_br = m_can_info.canValue[19] * 10.0;
        radar.long_dis_radar_fl = m_can_info.canValue[20] * 10.0;
        radar.long_dis_radar_fr = m_can_info.canValue[21] * 10.0;
        radar.long_dis_radar_bl = m_can_info.canValue[22] * 10.0;
        radar.long_dis_radar_br = m_can_info.canValue[23] * 10.0;
        radar_list->push_back(radar);
    }
}
void PraseByTs(std::deque<CanInfo>* list_can_info, uint64_t timestamp,
                std::deque<UltrasonicRadarInfo>* radar_list, std::deque<TrackReqAuxInfo>* aux_list,
                uint8_t ds_cal_mode,
                std::deque<CanInfo>* matched_can_values) {
    CanInfo t;
    t.ts = timestamp;
    double seconds = 0.0;
    auto ans = std::lower_bound(list_can_info->begin(), list_can_info->end(), t);
    CanInfo getans;
    if (ans != list_can_info->end()) {
        getans.canValue = ans->canValue;
        getans.ts = ans->ts;
        getans.speed = ans->speed;
        getans.turnlump = ans->turnlump;
    }
    if (list_can_info->size() > 0) {
        while (true) {
            if (list_can_info->size() == 0) {
                break;
            }
            CanInfo getans2 = list_can_info->front();
            std::string seconds_str = std::to_string(getans2.ts);
            // VLOG(1) << " str len = " << seconds_str.length();
            if (seconds_str.length() < 16) {
                list_can_info->pop_front();
            }
            if (getans2 <= t) {
                matched_can_values->push_back(getans2);
                TrackReqAuxInfo aux;
                UltrasonicRadarInfo radar;
                std::string seconds_str = std::to_string(getans2.ts);
                if (seconds_str.length() < 16) {
                    list_can_info->pop_front();
                }
                std::string temp = ".";
                // VLOG(1) << "seconds_str = " << seconds_str;
                seconds_str.insert(10, temp);
                std::istringstream ss_second(seconds_str);
                ss_second >> seconds;
                aux.tm_ms = static_cast<uint64_t>(seconds*1000);
                aux.ds_cal_mode = ds_cal_mode;
                // aux.tm_ms = getans2.ts;
                aux.speed_kmph = getans2.speed;
                aux.speed_kmph = 0.5*(getans2.canValue[28] + getans2.canValue[29]);
                aux.steer_deg = getans2.canValue[1];
                int direction = static_cast<int>(getans2.canValue[2]);
                if (direction == 1) {
                    aux.steer_deg *= -1;
                }
                aux.wheel_pulse_counter_fl = getans2.canValue[5];
                aux.wheel_pulse_counter_fr = getans2.canValue[7];
                aux.wheel_pulse_counter_rl = getans2.canValue[9];
                aux.wheel_pulse_counter_rr = getans2.canValue[11];
                aux.wheel_spd_fl = getans2.canValue[26];
                aux.wheel_spd_fr = getans2.canValue[27];
                aux.wheel_spd_rl = getans2.canValue[28];
                aux.wheel_spd_rr = getans2.canValue[29];

                aux.shift_mode = getans2.canValue[13];
                aux.driver_torque = (1.0 - getans2.canValue[4]) * getans2.canValue[3];
                aux.longitudinal_acceleration = getans2.canValue[32];
                aux.lateral_acceleration = getans2.canValue[33];
                if (getans2.canValue[31] == 1) {
                    aux.yaw_rate = getans2.canValue[30];
                } else {
                    aux.yaw_rate = -getans2.canValue[30];
                }
                aux_list->push_back(aux);
                radar.tm_ms = static_cast<uint64_t>(seconds*1000);
                radar.short_dis_radar_fl = getans2.canValue[14] * 10.0;
                radar.short_dis_radar_flm = getans2.canValue[15] * 10.0;
                radar.short_dis_radar_frm = getans2.canValue[16] * 10.0;
                radar.short_dis_radar_fr = getans2.canValue[17] * 10.0;
                radar.short_dis_radar_bl = getans2.canValue[18] * 10.0;
                radar.short_dis_radar_blm = getans2.canValue[19] * 10.0;
                radar.short_dis_radar_brm = getans2.canValue[20] * 10.0;
                radar.short_dis_radar_br = getans2.canValue[21] * 10.0;
                radar.long_dis_radar_fl = getans2.canValue[23] * 10.0;
                radar.long_dis_radar_fr = getans2.canValue[22] * 10.0;
                radar.long_dis_radar_bl = getans2.canValue[24] * 10.0;
                radar.long_dis_radar_br = getans2.canValue[25] * 10.0;
                radar_list->push_back(radar);
                list_can_info->pop_front();
            } else {
                break;
            }
        }
    }
}


