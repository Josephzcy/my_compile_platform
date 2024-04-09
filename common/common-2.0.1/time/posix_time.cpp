#include "posix_time.h"

#if defined(__unix__) || defined(__APPLE__)
#include <stddef.h>
#include <time.h>
#include <sys/time.h>

// namespace common {

// ts <- current_time + relative_time_in_ms
void RelativeTimeInMillSecondsToAbsTimeInTimeSpec(
    int64_t relative_time_in_ms,
    timespec* ts
    )
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t usec = tv.tv_usec + relative_time_in_ms * 1000LL;
    ts->tv_sec = tv.tv_sec + usec / 1000000;
    ts->tv_nsec = (usec % 1000000) * 1000;
}

// RelativeTimeInMillSecondsToAbsTimeInTimeSpec(relative_time_in_ms * 1000, ts);
void RelativeTimeInMicoSecondsToAbsTimeInTimeSpec(
    int64_t relative_time_in_us,
    timespec* ts
    )
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t usec = tv.tv_usec + relative_time_in_us;
    ts->tv_sec = tv.tv_sec + usec / 1000000;
    ts->tv_nsec = (usec % 1000000) * 1000;
}
// } // namespace common

#endif
