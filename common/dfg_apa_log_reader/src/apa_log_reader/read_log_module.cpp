// Copyright [2019] - MiniEye INC.
#include <stdint.h>
#include <string>
#include "apa_interface.h"
#include "apa_camera_log.h"
#include "apa_motion_log.h"
#include "read_log_module.h"

namespace log_reader {
    static ApaCameraLog* g_camera_reader_handle = NULL;
    static ApaMotionLog* g_motion_reader_handle = NULL;
}  // namespace log_reader

void ReadLogInit(const std::string& camera_log_file, const std::string& motion_log_file, uint8_t ds_cal_mode) {
    log_reader::g_camera_reader_handle = new log_reader::ApaCameraLog;
    log_reader::g_motion_reader_handle = new log_reader::ApaMotionLog;
    log_reader::g_camera_reader_handle->Init(camera_log_file);
    log_reader::g_motion_reader_handle->Init(motion_log_file, ds_cal_mode);
}

bool ReadLogRun(uint32_t frame_id, uint64_t* img_tm_ms, void* aux_info_list, void* radar_info_list) {
    std::deque<TrackReqAuxInfo>* aux_list = static_cast<std::deque<TrackReqAuxInfo>*>(aux_info_list);
    std::deque<UltrasonicRadarInfo>* radar_list = static_cast<std::deque<UltrasonicRadarInfo>*>(radar_info_list);
    uint64_t timestamp = 0;
    bool ret_camera_reader = log_reader::g_camera_reader_handle->MatchTimestamp(frame_id, &timestamp);
    if (ret_camera_reader) {
        bool ret_motion_reader = log_reader::g_motion_reader_handle->ReadMotionData(timestamp, aux_list, radar_list);
        if (ret_motion_reader) {
            *img_tm_ms = timestamp;
            return true;
        }
    }
    return false;
}

void ReadLogDestroy() {
    delete log_reader::g_camera_reader_handle;
    log_reader::g_camera_reader_handle = NULL;
    delete log_reader::g_motion_reader_handle;
    log_reader::g_motion_reader_handle = NULL;
}
