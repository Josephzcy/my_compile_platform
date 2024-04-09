// Copyright [2019] - MiniEye INC.
//#include "common/concurrency/rwlock.h"
#include "common/concurrency/work_thread.h"
#include "common/time/time_utils.h"
#include "common/concurrency/this_thread.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "apa_interface/apa_interface.h"
#include "read_log_module.h"
#include "json.hpp"

DEFINE_string(birdview_video_file, "", "birdview video file");
DEFINE_string(birdview_images_file, "", "birdview images file");
DEFINE_string(apa_motion_log_addr, "./motion.txt", "apa motion log address");
DEFINE_string(apa_camera_log_addr, "./camera.txt", "apa camera log address");
DEFINE_int32(start_frame_no, 1, "xx");
DEFINE_int32(end_frame_no, 6000, "xx");
DEFINE_bool(generate_pic, false, "xx");

std::string num2str(int i) {
    // char ss[10];
    // snprintf(ss, "%07d", i);
    // return ss;
}

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetLogDestination(google::GLOG_INFO, "./log/loger");
    google::SetLogDestination(google::GLOG_ERROR, "./log/loger");
    FLAGS_colorlogtostderr = true;
    cv::VideoCapture birdview_cap(FLAGS_birdview_video_file);
    if (!birdview_cap.isOpened()) {
        LOG(ERROR) << "open video failed : " << FLAGS_birdview_video_file;
        return -1;
    }
    ReadLogInit(FLAGS_apa_camera_log_addr, FLAGS_apa_motion_log_addr, 0);
    int frame_num = birdview_cap.get(cv::CAP_PROP_FRAME_COUNT);
    for (int i = 0; i < frame_num; i++) {
        LOG(ERROR) << " frame_id = " << i;
        if (i < FLAGS_start_frame_no) continue;

        std::deque<TrackReqAuxInfo> aux_list;
        std::deque<UltrasonicRadarInfo> radar_list;
        UltrasonicRadarInfo radar_info;
        TrackReqAuxInfo aux_info;
        uint64_t img_tm = 0;
        bool ret_read = ReadLogRun(static_cast<int>(i), &img_tm, static_cast<void*>(&aux_list),
                                    static_cast<void*>(&radar_list));
        if (!ret_read){
            LOG(ERROR) << "read log failed!";
        } else {
            radar_info = radar_list.back();
            aux_info = aux_list.back();
            LOG(INFO) << "time:" << radar_info.tm_ms << "...fl: " << radar_info.long_dis_radar_fl
                      << "...bl: " << radar_info.long_dis_radar_bl << "...fr: " << radar_info.long_dis_radar_fr
                      << "...br: " << radar_info.long_dis_radar_br << "   velo:" << aux_info.speed_kmph;
            cv::Mat frame;
            frame = cv::imread(FLAGS_birdview_images_file + std::to_string(i) + ".jpg");
            std::string draw_str = "";
            /*
                putText(图片，字符串，位置，字体，尺寸，线条颜色，线条宽度，线型，××)
            */    
            draw_str = "upa_fl" + std::to_string((int)radar_info.short_dis_radar_fl);
            cv::putText(frame, draw_str, cv::Point(820, 240), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,200), 2, 8, false);
            draw_str = "upa_flm" + std::to_string((int)radar_info.short_dis_radar_flm);
            cv::putText(frame, draw_str, cv::Point(930, 240), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,200), 2, 8, false);
            draw_str = "upa_frm" + std::to_string((int)radar_info.short_dis_radar_frm);
            cv::putText(frame, draw_str, cv::Point(1050, 240), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,200), 2, 8, false);
            draw_str = "upa_fr" + std::to_string((int)radar_info.short_dis_radar_fr);
            cv::putText(frame, draw_str, cv::Point(1180, 240), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,200), 2, 8, false);
            draw_str = "upa_bl" + std::to_string((int)radar_info.short_dis_radar_bl);
            cv::putText(frame, draw_str, cv::Point(820, 480), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,200), 2, 8, false);
            draw_str = "upa_blm" + std::to_string((int)radar_info.short_dis_radar_blm);
            cv::putText(frame, draw_str, cv::Point(930, 480), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,200), 2, 8, false);
            draw_str = "upa_brm" + std::to_string((int)radar_info.short_dis_radar_brm);
            cv::putText(frame, draw_str, cv::Point(1050, 480), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,200), 2, 8, false); 
            draw_str = "upa_br" + std::to_string((int)radar_info.short_dis_radar_br);
            cv::putText(frame, draw_str, cv::Point(1180, 480), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,200), 2, 8, false);
            /*
                在图像上显示8个UPA探头的探距信息
            */                                   
            draw_str = "apa_fl:" + std::to_string((int)radar_info.long_dis_radar_fl);
            cv::putText(frame, draw_str, cv::Point(840, 300), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,255), 2, 8, false);
            draw_str = "apa_bl:" + std::to_string((int)radar_info.long_dis_radar_bl);
            cv::putText(frame, draw_str, cv::Point(840, 420), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,255), 2, 8, false);
            draw_str = "apa_fr:" + std::to_string((int)radar_info.long_dis_radar_fr);
            cv::putText(frame, draw_str, cv::Point(1080, 300), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,255), 2, 8, false);
            draw_str = "apa_br:" + std::to_string((int)radar_info.long_dis_radar_br);
            cv::putText(frame, draw_str, cv::Point(1080, 420), cv::FONT_HERSHEY_PLAIN,
                        1.0, cv::Scalar(0,0,255), 2, 8, false);
            if (FLAGS_generate_pic) {
                std::string file_name;
                file_name = "result/" + std::to_string(i) + ".jpg";
                cv::imwrite(file_name, frame);
            }
            radar_list.pop_back();
            aux_list.pop_back();
        }
    }
    LOG(INFO) << "Finish!";
    return 0;
}
