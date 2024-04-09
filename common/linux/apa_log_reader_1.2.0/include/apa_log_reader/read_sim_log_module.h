// Copyright [2019] - MiniEye INC.
#pragma once
#include <stdint.h>
#include <string>
#ifdef __cplusplus
extern "C" {
#endif



void ReadSimLogInit(const std::string& camera_log_file,
                const std::string& motion_log_file,
                const std::string& ultrasonic_log_file);

bool ReadSimLogRun(uint32_t frame_id, uint64_t* img_tm_ms,
                void* aux_info_list, void* radar_info_list,
                void* benchmark_info_list);

void ReadSimLogDestroy();

#ifdef __cplusplus
};
#endif
