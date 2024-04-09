#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <thread>
#include <opencv2/opencv.hpp>
#include "flow.hpp"

static flow::Context g_flow;
static const char* video_path = nullptr;
static int fps = 25;

static void sleep_us(int us) {
  struct timespec ts;
  ts.tv_sec = us / 1000000;
  ts.tv_nsec = (us - ts.tv_sec * 1000000) * 1000;
  nanosleep(&ts, nullptr);
}

static int64_t get_current_time_us() {
  struct timeval tv;

  int rc = gettimeofday(&tv, nullptr);
  if (rc == -1) {
    perror("gettimeofday");
    return 0;
  }

  return tv.tv_sec * 1000000 + tv.tv_usec;
}

void func_th1(int fps) {
  flow::Sender sender(&g_flow, "th1");

  cv::VideoCapture cap(video_path);  // open the video file
  if (!cap.isOpened()) {  // check if we succeeded
    fprintf(stderr, "Error: failed to open video file\n");
    exit(1);
  }

  int64_t frame_interval = 1000000 / fps;
  int i = 0;

  while (true) {
    int64_t start_time = get_current_time_us();

    cv::Mat frame;
    cap >> frame;  // read a new frame
    if (frame.empty()) {
      break;
    }

    size_t data_size = frame.cols * frame.rows * frame.elemSize();
    sender.send("image", (const char*)frame.data, data_size);

    fprintf(stderr, "frame %d\n", i);
    if (i++ == 0) {
      fprintf(stderr, "frame size: %dx%d\n", frame.cols, frame.rows);
    }

    int64_t end_time = get_current_time_us();
    int64_t elapsed = end_time - start_time;
    int64_t sleep_time_us = frame_interval - elapsed;
    if (sleep_time_us > 0) {
      sleep_us(sleep_time_us);
    }
  }

  fprintf(stderr, "the th1 thread is done\n");
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s video_path [fps=25]\n", argv[0]);
    exit(1);
  }

  video_path = argv[1];

  if (argc >= 3) {
    fps = atoi(argv[2]);
    if (fps <= 0) {
      fprintf(stderr, "Error: invalid fps: %d\n", fps);
      exit(1);
    }
  }

  if (g_flow.start() != 0) {
    exit(1);
  }

  std::thread th(func_th1, fps);
  th.join();

  return 0;
}
