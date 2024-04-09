// Copyright [2019] - MiniEye INC.
#pragma once
#include <stdint.h>
#include <string>
#ifdef __cplusplus
extern "C" {
#endif

void ReadLogInit(const std::string& camera_log_file,
                const std::string& motion_log_file,
                uint8_t ds_cal_mode);

bool ReadLogRun(uint32_t frame_id, uint64_t* img_tm_ms,
                void* aux_info_list, void* radar_info_list);

void ReadLogDestroy();

#ifdef __cplusplus
};
#endif
