// Copyright [2022] - MiniEye INC.
#include "aipilot/services/bird_eye_view/utils/common_utils.h"

#include <sys/time.h>

#include <cstddef>

namespace common_utils {
int64_t GetTimeMs() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

}  // namespace common_utils
