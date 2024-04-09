// Copyright [2022] - MiniEye INC.
#ifndef BEV_UTILS_FPS_H_
#define BEV_UTILS_FPS_H_

#include <chrono>
#include <string>

#include "glog/logging.h"

namespace minieye {
namespace fps {

class CFPS {
 public:
  explicit CFPS(const std::string &name) : name_(name) { pre_time_ = std::chrono::steady_clock::now(); }

  void Triger() {
    ++count_;

    auto cur_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(cur_time - pre_time_).count();
    if (elapsed > 500) {
      fps_ = (count_ - pre_count_) * 1e3 / elapsed;
      pre_time_ = cur_time;
      pre_count_ = count_;
    }

    LOG(INFO) << "[" << name_ << "] fps = " << fps_;
  }

 private:
  std::string name_;
  int count_ = 0;
  int pre_count_ = 0;
  double fps_ = 0.0;
  std::chrono::time_point<std::chrono::steady_clock> pre_time_;
};

}  // namespace fps
}  // namespace minieye

#endif  // BEV_UTILS_FPS_H_
