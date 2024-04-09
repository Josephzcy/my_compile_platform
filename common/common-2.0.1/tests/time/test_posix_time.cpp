/// Copyright (C) 2015 - MiniEye INC.

#include <gtest/gtest.h>
#include <ctime>
#include "common/time/posix_time.h"

inline int64_t milliseconds(int seconds) {
    return seconds * 1000;
}

inline int64_t microseconds(int seconds) {
    return seconds * 1000000;
}

TEST(TestPosixTime, Test1) {
    int64_t now = std::time(nullptr);  // seconds from epoch
    int deltas[] = {0, 1, 10, 100};
    for (size_t i = 0; i < sizeof(deltas) / sizeof(deltas[0]); ++i) {
        int64_t reltime = milliseconds(deltas[i]);
        struct timespec ts;
        RelativeTimeInMillSecondsToAbsTimeInTimeSpec(reltime, &ts);
        ASSERT_EQ(now + deltas[i], ts.tv_sec);
    }
}

TEST(TestPosixTime, Test2) {
    int64_t now = std::time(nullptr);  // seconds from epoch
    int deltas[] = {0, 1, 10, 100};
    for (size_t i = 0; i < sizeof(deltas) / sizeof(deltas[0]); ++i) {
        int64_t reltime = microseconds(deltas[i]);
        struct timespec ts;
        RelativeTimeInMicoSecondsToAbsTimeInTimeSpec(reltime, &ts);
        ASSERT_EQ(now + deltas[i], ts.tv_sec);
    }
}
