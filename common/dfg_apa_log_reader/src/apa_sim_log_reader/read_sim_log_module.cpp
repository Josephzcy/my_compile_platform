// Copyright [2019] - MiniEye INC.
#include <stdint.h>
#include <string>
#include "apa_interface.h"
#include "apa_camera_sim_log.h"
#include "apa_motion_sim_log.h"
#include "apa_ultrasonic_sim_log.h"
#include "read_sim_log_module.h"

namespace sim_log_reader {
    static ApaCameraLog* g_camera_reader_handle = NULL;
    static ApaMotionLog* g_motion_reader_handle = NULL;
    static ApaUltrasonicLog* g_ultrasonic_reader_handle = NULL;
}  // namespace sim_log_reader

void ReadSimLogInit(const std::string& camera_log_file,
                const std::string& motion_log_file,
                const std::string& ultrasonic_log_file) {
    sim_log_reader::g_camera_reader_handle = new sim_log_reader::ApaCameraLog;
    sim_log_reader::g_motion_reader_handle = new sim_log_reader::ApaMotionLog;
    sim_log_reader::g_ultrasonic_reader_handle = new sim_log_reader::ApaUltrasonicLog;
    sim_log_reader::g_camera_reader_handle->Init(camera_log_file);
    sim_log_reader::g_motion_reader_handle->Init(motion_log_file);
    sim_log_reader::g_ultrasonic_reader_handle->Init(ultrasonic_log_file);
}

bool ReadSimLogRun(uint32_t frame_id, uint64_t* img_tm_ms,
                void* aux_info_list, void* radar_info_list,
                void* benchmark_info_list) {
    std::deque<TrackReqAuxInfo>* aux_list =
        static_cast<std::deque<TrackReqAuxInfo>*>(aux_info_list);
    std::deque<UltrasonicRadarInfo>* radar_list =
        static_cast<std::deque<UltrasonicRadarInfo>*>(radar_info_list);
    std::deque<sim_log_reader::BenchMark>* benchmark_list =
        static_cast<std::deque<sim_log_reader::BenchMark>*>(benchmark_info_list);
    uint64_t timestamp = 0;
    bool ret_camera_reader =
        sim_log_reader::g_camera_reader_handle->MatchTimestamp(
            frame_id, &timestamp);
    if (ret_camera_reader) {
        bool ret_motion_reader =
            sim_log_reader::g_motion_reader_handle->ReadMotionData(
                timestamp, aux_list, benchmark_list);
        if (ret_motion_reader) {
            bool ret_ultrasonic_reader =
                sim_log_reader::g_ultrasonic_reader_handle->ReadUltrasonicData(
                    timestamp, radar_list);
            if (ret_ultrasonic_reader) {
                *img_tm_ms = timestamp;
                return true;
            }
        }
    }
    return false;
}

void ReadSimLogDestroy() {
    delete sim_log_reader::g_camera_reader_handle;
    sim_log_reader::g_camera_reader_handle = NULL;
    delete sim_log_reader::g_motion_reader_handle;
    sim_log_reader::g_motion_reader_handle = NULL;
    delete sim_log_reader::g_ultrasonic_reader_handle;
    sim_log_reader::g_ultrasonic_reader_handle = NULL;
}
