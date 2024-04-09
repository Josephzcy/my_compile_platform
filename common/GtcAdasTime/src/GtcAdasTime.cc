#include "GtcAdasTime.h"
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>

uint64_t GetAdsTimeInUsecs(void)
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (tp.tv_sec * 1000000) + (tp.tv_nsec / 1000);
}

uint64_t tick_to_utc_time_in_us(uint64_t tick_us)
{
    struct timeval tv;
    uint64_t gap_tick;

    gettimeofday(&tv, NULL);
    gap_tick = GetAdsTimeInUsecs() - tick_us;

    return (tv.tv_sec * 1000000) + tv.tv_usec - gap_tick;
}

uint64_t get_utc_time_in_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}
