// Copyright [2019] - MiniEye INC.
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include "json.hpp"
#include "opencv2/opencv.hpp"
#include "msgpack.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "apa_interface.h"
#include "json_reader.h"

DEFINE_string(json_file, "", "test file");
DEFINE_string(fisheye_video_file, "", "fisheye_video_file");
DEFINE_string(apa_camera_log_addr, "", "");

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetLogDestination(google::GLOG_INFO, "./log/Info");
    google::SetLogDestination(google::GLOG_ERROR, "./log/Error");
    std::vector<CanInfo> result;
    std::deque<TrackReqAuxInfo> aux_list;
    std::deque<UltrasonicRadarInfo> radar_list;
    UltrasonicRadarInfo radar;
    TrackReqAuxInfo aux;
    ReadJsonLog(FLAGS_json_file, &result, &aux_list, &radar_list);
    cv::VideoCapture cap(FLAGS_fisheye_video_file);
    if (!cap.isOpened()) {
        LOG(ERROR) << "open video failed : " << FLAGS_fisheye_video_file;
        return -1;
    }
    std::ifstream getstring(FLAGS_apa_camera_log_addr);
    FILE* f = fopen(FLAGS_apa_camera_log_addr.c_str(), "r");
    int now_data = 0;
    int frame_num = cap.get(cv::CAP_PROP_FRAME_COUNT);
    for (int frame_id = 0; frame_id < frame_num; frame_id++) {
        cv::Mat frame;
        cap >> frame;
        std::string file_name;
        char fuck[50];
        int a, b;
        fscanf(f, "%d%d%s%s%s", &a, &b, fuck, fuck, fuck);
        uint64_t ts = static_cast<uint64_t>(a) * 1000000LL + b;
        PraseByTs(&result, ts, radar, aux);
        LOG(ERROR) << "frame_id : " << frame_id <<"     time_stamp : " << ts;
        LOG(ERROR) << "ts : " << radar.tm_ms <<"     radar : " << radar.long_dis_radar_bl;
        LOG(ERROR) << "ts : " << aux.tm_ms <<"     spd : " << aux.speed_kmph;
    }
}