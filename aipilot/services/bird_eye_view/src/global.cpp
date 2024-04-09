// Copyright [2022] - MiniEye INC.
#include "aipilot/services/bird_eye_view/src/global.h"

DEFINE_uint32(width, 2560, "input image width");
DEFINE_uint32(height, 1600, "input image height");
DEFINE_uint32(output_width, 352, "output image width");
DEFINE_uint32(output_height, 480, "output image height");
DEFINE_string(dds_config, "/app/algo/apa/topic/topic.json", "dds_config file name");
DEFINE_string(dds_image_topic, "camera30", "dds image topic");
DEFINE_string(output_dds_image_topic, "camera41", "output frame to dds topic");
DEFINE_string(dds_vehicle_signal_topic, "vehicle_signal", "dds vehicle_signal topic");
DEFINE_string(dds_apa_state_control_topic, "apa_state_control", "dds apa_state_control topic");
DEFINE_string(output_frame_dir, "", "output frame to directory");
DEFINE_string(input_frame_dir, "", "input frame to directory");
DEFINE_uint32(bev_buffer_num, 8, "bev buffer number");
DEFINE_bool(save_timestamp, false, "whether to save timestamp, for HIL-SIM use");
DEFINE_bool(is_init_in_apa_state, false, "set to true, for no apa_state dds use");
DEFINE_bool(is_init_door_close, false, "set to true, for no vehicle signal dds use");
DEFINE_double(max_fps, 12.5, "max fps");
DEFINE_string(lut_file, "/calib/avm_calib/avm_dsp_lut.bin", "avmmapping lut file");
DEFINE_bool(save_bev_nv12_format, false, "whether to save bev in NV12 format");
DEFINE_string(car_info_file, "/calib/avm_calib/bev_car_info.json", "car info file name");
DEFINE_double(half_width_phycical_dis, 6.0, "half bev width's physical distance");
