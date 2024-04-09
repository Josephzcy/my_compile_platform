// Copyright [2019] - MiniEye INC.
#include <string>
#include <deque>
#include "common/concurrency/work_thread.h"
#include "common/time/time_utils.h"
//#include "common/concurrency/rwlock.h"
#include "common/concurrency/this_thread.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "apa_interface.h"
#include "rpc/client.h"
#include "read_sim_log_module.h"
#include "apa_motion_sim_log.h"
#include "json.hpp"

DEFINE_string(birdview_video_file, "", "birdview video file");
DEFINE_string(birdview_images_file, "", "birdview images file");
DEFINE_string(apa_motion_log_addr, "./log.txt", "apa motion log address");
DEFINE_string(apa_camera_log_addr, "./log_birdview.txt", "apa camera log address");
DEFINE_string(apa_ultrasonic_log_addr, "./log_ultrasonic.txt", "apa ultrasonic log address");
DEFINE_int32(start_frame_no, 1, "xx");
DEFINE_int32(end_frame_no, 6000, "xx");
DEFINE_bool(generate_pic, false, "xx");

int main(int argc, char *argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetLogDestination(google::GLOG_INFO, "./log/Info");
    google::SetLogDestination(google::GLOG_ERROR, "./log/Error");
    ReadSimLogInit(FLAGS_apa_camera_log_addr, FLAGS_apa_motion_log_addr,
                FLAGS_apa_ultrasonic_log_addr);
    cv::VideoCapture birdview_cap(FLAGS_birdview_video_file);
    if (!birdview_cap.isOpened()) {
        LOG(ERROR) << "open video failed : " << FLAGS_birdview_video_file;
        return -1;
    }
    LOG(ERROR) << "UltraParkingspot finished!";
    int frame_num = birdview_cap.get(cv::CAP_PROP_FRAME_COUNT);
    //  int frame_num = FLAGS_end_frame_no;
    for (int i = 0; i < frame_num; i++) {
        LOG(ERROR) << " frame_id = " << i;
        if (i < FLAGS_start_frame_no)
            continue;

        std::deque<TrackReqAuxInfo> aux_list;
        std::deque<UltrasonicRadarInfo> radar_list;
        std::deque<sim_log_reader::BenchMark> benchmark_list;
        UltrasonicRadarInfo radar_info;
        TrackReqAuxInfo aux_info;
        sim_log_reader::BenchMark benchmark_info;
        ParkingSpace res_left;
        ParkingSpace res_right;
        uint64_t img_tm = 0;
        bool ret_read = ReadSimLogRun(static_cast<int>(i), &img_tm,
                                    static_cast<void *>(&aux_list),
                                   static_cast<void *>(&radar_list),
                                   static_cast<void *>(&benchmark_list));
        if (!ret_read) {
            LOG(ERROR) << "read log failed!";
        } else {
            radar_info = radar_list.back();
            aux_info = aux_list.back();
            benchmark_info = benchmark_list.back();
            LOG(INFO) << "time:" << radar_info.tm_ms
                      << "...fl: " << radar_info.long_dis_radar_fl
                      << "...bl: " << radar_info.long_dis_radar_bl
                      << "...fr: " << radar_info.long_dis_radar_fr
                      << "...br: " << radar_info.long_dis_radar_br;
            LOG(INFO) << "time:" << benchmark_info.veh_loc.tm
                      << "...veh_loc_x: " << benchmark_info.veh_loc.x
                      << "...veh_loc_y: " << benchmark_info.veh_loc.y
                      << "...obj_loc_a_x_re: " <<
                      benchmark_info.obj_loc_a.x_relation
                      << "...obj_loc_a_y_re: " <<
                      benchmark_info.obj_loc_a.y_relation
                      << "...obj_loc_b_x_re: " <<
                      benchmark_info.obj_loc_b.x_relation
                      << "...obj_loc_b_y_re: " <<
                      benchmark_info.obj_loc_b.y_relation
                      << "...obj_loc_c_x_re: " <<
                      benchmark_info.obj_loc_c.x_relation
                      << "...obj_loc_c_y_re: " <<
                      benchmark_info.obj_loc_c.y_relation;
            LOG(INFO) << "time:" << aux_info.tm_ms
                      << "...velo: " << aux_info.speed_kmph
                      << "...wc_rl: " << aux_info.wheel_pulse_counter_rl
                      << "...wc_rr: " << aux_info.wheel_pulse_counter_rr
                      << "...steer: " << aux_info.steer_deg;
                  }
              }
    LOG(INFO) << "Finish!";
    return 0;
}
