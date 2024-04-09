#ifndef COMMON_TIME_TIMESTAMP_H
#define COMMON_TIME_TIMESTAMP_H

#include "common/base/stdint.h"

/// time stamp in millisecond (1/1000 second)
int64_t GetTimeStampInMs();

/// time stamp in microsecond (1/1000000 second)
int64_t GetTimeStampInUs();

// } // namespace common

#endif // COMMON_TIME_TIMESTAMP_H
