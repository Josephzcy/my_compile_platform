/// Copyright (C) 2015 - MiniEye INC.

#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include "common/time/timestamp.h"

// sleep for several milliseconds
inline void msleep(int milliseconds) {
    struct timespec req;
    req.tv_sec = milliseconds / 1000;
    req.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&req, nullptr);
}

TEST(TestTimestamp, TestGetTimeStampInMs) {
    for (int i = 0; i < 10; ++i) {
        int64_t time_s = int64_t{std::time(nullptr)};  // seconds from epoch
        int64_t time_ms = GetTimeStampInMs();

        // the difference between time_s and time_ms shall <= 1s
        ASSERT_LE(std::abs(time_s - time_ms / 1000), 1);

        msleep(10);
    }
}

TEST(TestTimestamp, TestGetTimeStampInUs) {
    for (int i = 0; i < 10; ++i) {
        int64_t time_ms = GetTimeStampInMs();
        int64_t time_us = GetTimeStampInUs();

        // the difference between time_ms and time_us shall <= 10ms
        ASSERT_LE(std::abs(time_ms - time_us / 1000), 10);

        msleep(10);
    }
}
