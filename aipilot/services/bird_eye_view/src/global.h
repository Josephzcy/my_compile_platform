// Copyright [2022] - MiniEye INC.
#ifndef BEV_SRC_GLOBAL_H_
#define BEV_SRC_GLOBAL_H_

#include "gflags/gflags.h"

// 输入图像的大小
DECLARE_uint32(width);
DECLARE_uint32(height);

// 输出图像的大小
DECLARE_uint32(output_width);   // 宽度
DECLARE_uint32(output_height);  // 高度

// dds 的配置文件 config.json
DECLARE_string(dds_config);

// 从这个 dds topic 中获取 minieye::CameraFrame 数据
DECLARE_string(dds_image_topic);

// 发送渲染完的buffer到该topic
DECLARE_string(output_dds_image_topic);

// vehicle_signal topic name
DECLARE_string(dds_vehicle_signal_topic);

// apa_state_control
DECLARE_string(dds_apa_state_control_topic);

// 测试使用，保存渲染完的buffer到output_frame_dir目录
DECLARE_string(output_frame_dir);

// 保存输入的四路鱼眼图
DECLARE_string(input_frame_dir);

DECLARE_uint32(bev_buffer_num);

// HIL-SIM: ts and tick exchanged
DECLARE_bool(save_timestamp);

// is in apa state initially
DECLARE_bool(is_init_in_apa_state);

// is door closed initially
DECLARE_bool(is_init_door_close);

// frame rate control
DECLARE_double(max_fps);

DECLARE_string(lut_file);

// 保存bev的NV12格式原始数据
DECLARE_bool(save_bev_nv12_format);

// car info file
DECLARE_string(car_info_file);

// bev图一半宽度对应物理距离，视野范围6m
DECLARE_double(half_width_phycical_dis);
#endif  // BEV_SRC_GLOBAL_H_
