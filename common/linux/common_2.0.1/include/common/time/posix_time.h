#ifndef COMMON_SYSTEM_TIME_POSIX_TIME_H
#define COMMON_SYSTEM_TIME_POSIX_TIME_H

#if defined(__unix__) || defined(__APPLE__)

#include "common/base/stdint.h"

struct timespec;

// for any timed* functions using absolute timespec
void RelativeTimeInMillSecondsToAbsTimeInTimeSpec(
    int64_t relative_time_in_ms,
    timespec* ts
    );

void RelativeTimeInMicoSecondsToAbsTimeInTimeSpec(
        int64_t relative_time_in_us,
        timespec* ts
        );

#else
//#error for POSIX compatible platforms only
#endif


#endif // COMMON_SYSTEM_TIME_POSIX_TIME_H
