// Copyright [2019] - MiniEye INC.
#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <deque>
#include "apa_interface.h"
#ifdef __cplusplus
extern "C" {
#endif

void ReadJsonLog(const std::string& filelist,
                    std::deque<CanInfo>* result,
                    std::deque<TrackReqAuxInfo>* aux_list,
                    std::deque<UltrasonicRadarInfo>* radar_list);
void PraseByTs(std::deque<CanInfo>* list_can_info, uint64_t timestamp,
                std::deque<UltrasonicRadarInfo>* radar_info, std::deque<TrackReqAuxInfo>* aux_info,
                uint8_t ds_cal_mode,
                std::deque<CanInfo>* matched_can_values);
#ifdef __cplusplus
};
#endif
