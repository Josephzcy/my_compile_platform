/// Copyright (C) 2019 - MiniEye INC.

#include "./client1.h"
#include <sys/time.h>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <opencv2/opencv.hpp>

MTQueue<std::shared_ptr<msgpack::object_handle>> ImageVec;
int64_t LastFPSTime = -1;
int64_t FrameCounter = 0;
float FPS = 0.0;

static int64_t get_current_time_us() {
  struct timeval tv;

  int rc = gettimeofday(&tv, nullptr);
  if (rc == -1) {
    perror("gettimeofday");
    return 0;
  }

  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char* argv[]) {
  std::thread th1(RecvThreadFunc);
  th1.detach();

  std::string window_name = "Video";
  if (argc > 1) {
    window_name += std::string(": ") + argv[1];
  }

  cv::namedWindow(window_name.c_str(), 1);

  while (true) {
    cv::Size dim(1280, 720);
    cv::Mat image;

    std::shared_ptr<msgpack::object_handle> oh;
    int ret = ImageVec.Pull(&oh, 1000);
    if (ret != 0 || !oh) {
      continue;
    }

    msgpack::object deserialized = oh->get();
    std::map<std::string, msgpack::object> dst;
    deserialized.convert(dst);

    const std::string& str = dst["data"].as<std::string>();
    image = cv::Mat(dim, CV_8UC3, (char*)str.c_str());

    if (image.empty()) {
      continue;
    }

    // compute FPS
    FrameCounter += 1;
    int64_t curr_time = get_current_time_us();
    int64_t dt = curr_time - LastFPSTime;
    if (dt >= 500 * 1000) {
      FPS = 1e6 * FrameCounter / dt;
      LastFPSTime = curr_time;
      FrameCounter = 0;
    }

    // draw FPS
    char buf[32];
    snprintf(buf, sizeof(buf), "FPS: %.2f", FPS);
    auto pos = cv::Point(40, 40);
    auto color = CvScalar(0, 0, 200);
    cv::putText(image, buf, pos, CV_FONT_HERSHEY_DUPLEX,
      1.0, color, 1, CV_AA);

    // show image
    cv::imshow(window_name.c_str(), image);
    int key = cv::waitKey(1) & 0xff;
    if (key == 'q' || key == 'Q') {  // quit
      break;
    }
  }

  return 0;
}
