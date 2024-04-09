// Copyright [2019] - MiniEye INC.
#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include "coordinate_mapping.h"
#include "common/time/timestamp.h"

DEFINE_string(config_file, "", "config filename");
DEFINE_string(video_file, "", "video file name");
DEFINE_double(x_start, -8000, "x start");
DEFINE_double(y_start, -8000, "y start");
DEFINE_double(y_end, 8000, "y end");
DEFINE_double(x_end, 8000, "y end");
DEFINE_double(xres, 40, "x res");
DEFINE_double(yres, 40, "y res");
DEFINE_double(vehicle_length, 4500, "vehicle length");

#define IMG_WIDTH 1280
#define IMG_HEIGHT 720

int GetCameraIndex(float x, float y) {
    float half_vehicle = FLAGS_vehicle_length / 2;
    if (y > half_vehicle) {
        return 0;
    } else if (y < -half_vehicle) {
        return 1;
    } else if (x < 0) {
        return 2;
    } else {
        return 3;
    }
}

int main(int argc, char *argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    apa::CoordinateMapping::Instance().Init(FLAGS_config_file);
    cv::VideoCapture cap(FLAGS_video_file);
    if (!cap.isOpened()) {
        LOG(ERROR) << "open video failed : " << FLAGS_video_file;
        return -1;
    }
    int frame_num = cap.get(cv::CAP_PROP_FRAME_COUNT);
    int cols = (FLAGS_x_end - FLAGS_x_start) / FLAGS_xres;
    int rows = (FLAGS_y_end - FLAGS_y_start) / FLAGS_yres;

    for (int i = 0; i < frame_num; ++i) {
        cv::Mat bird_img = cv::Mat::zeros(rows, cols, CV_8UC3);
        cv::Mat frame;
        cap >> frame;
        cv::Mat images[4];
        images[0] = frame(cv::Rect(0, 0, IMG_WIDTH, IMG_HEIGHT));
        images[1] = frame(cv::Rect(IMG_WIDTH, 0, IMG_WIDTH, IMG_HEIGHT));
        images[2] = frame(cv::Rect(0, IMG_HEIGHT, IMG_WIDTH, IMG_HEIGHT));
        images[3] = frame(cv::Rect(IMG_WIDTH, IMG_HEIGHT, IMG_WIDTH, IMG_HEIGHT));
        float x = FLAGS_x_start;
        float y = FLAGS_y_end;
        for (int r = 0; r < rows; ++r, y -= FLAGS_yres) {
            x = FLAGS_x_start;
            for (int c = 0; c < cols; ++c, x += FLAGS_xres) {
                int camera_index = GetCameraIndex(x, y);
                cv::Point2f raw_pt;
                bool ret = apa::CoordinateMapping::Instance().World2Raw(camera_index,
                        cv::Point2f(x, y),
                        &raw_pt);
                if (ret) {
                    if (raw_pt.y < 0 || raw_pt.y >= IMG_HEIGHT || raw_pt.x < 0 || raw_pt.x >= IMG_WIDTH)
                        continue;
                    cv::Vec3b val = images[camera_index].at<cv::Vec3b>(raw_pt.y, raw_pt.x);
                    bird_img.at<cv::Vec3b>(r, c) = val;
                }
            }
        }

        cv::imshow("front", images[0]);
        cv::imshow("rear", images[1]);
        cv::imshow("left", images[2]);
        cv::imshow("right", images[3]);
        cv::imshow("bird", bird_img);
        char key = cv::waitKey(5);
        if (key == ' ') {
            cv::waitKey(-1);
        }
    }
    return 0;
}
