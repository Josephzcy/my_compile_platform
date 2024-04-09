// Copyright [2019] - MiniEye INC.
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int bev_width;
  int bev_height;
  float trans_scale;  // bev_width/sight_width
  float car_length;
  float car_width;
  float car_wheelbase;
  float car_wheeltrack;
  float expand_l;
  float expand_w;
  float wheel_counter_ratio;
} ApaTransparentSetting;

void TransparentInit(int pipeline_num);

/**
 * 输出变换矩阵及对应的范围
 * @param camera_frame, camera30
 * 获取的图像信息，此处仅用来获取拼接图对应的时间戳
 * @param vehicle_signals， 车身信息数组
 * @param signal_length， 车身信息数组长度
 * @param expand_car_rect， 车身外扩后的区域
 * @param ref_rect_arr， 上一帧鸟瞰图用于变换的区域
 * @param dst_rect_arr， ref_rect 变换后的区域
 * @param warp_mat， 变换矩阵
 * @return
 */
bool TransparentGetWarp(void *camera_frame, void *vehicle_signals, int signal_length, float expand_car_rect[4],
                        float ref_rect_arr[4], float dst_rect_arr[4], float warp_mat[2][3]);

void TransparentDestroy();

/**
 * 可重置车身信息
 * @param change_res
 */
void TransparentReset(bool change_res, const ApaTransparentSetting *transparent_setting);

#ifdef __cplusplus
};
#endif
