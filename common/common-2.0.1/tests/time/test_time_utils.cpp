/// Copyright (C) 2015 - MiniEye INC.

#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "common/time/time_utils.h"

// timestamp: e.g. 20160616173521
static void check_timestamp(int64_t timestamp) {
    time_t now;
    std::time(&now);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

    int sec = timestamp % 100;

    timestamp /= 100;
    int min = timestamp % 100;

    timestamp /= 100;
    int hour = timestamp % 100;

    timestamp /= 100;
    int date = timestamp % 100;

    timestamp /= 100;
    int mon = timestamp % 100;

    timestamp /= 100;
    int year = timestamp;

    ASSERT_EQ(sec, timeinfo.tm_sec);
    ASSERT_EQ(min, timeinfo.tm_min);
    ASSERT_EQ(hour, timeinfo.tm_hour);
    ASSERT_EQ(date, timeinfo.tm_mday);
    ASSERT_EQ(mon, timeinfo.tm_mon + 1);  // tm_mon: [0, 11]
    ASSERT_EQ(year, timeinfo.tm_year + 1900);  // tm_year: since 1900
}

TEST(TestTimeUtils, TestMilliseconds) {
    int64_t time_s = int64_t{std::time(nullptr)};  // seconds from epoch
    int64_t time_ms = TimeUtils::Milliseconds();
    ASSERT_EQ(time_s, time_ms / 1000);
}

TEST(TestTimeUtils, TestMicroseconds) {
    int64_t time_ms = TimeUtils::Milliseconds();
    int64_t time_us = TimeUtils::Microseconds();
    ASSERT_EQ(time_ms, time_us / 1000);
}

TEST(TestTimeUtils, TestGetGMTOffset) {
    int offset1 = TimeUtils::GetGMTOffset();
    int offset2 = 8;  // Beijing time
    ASSERT_EQ(offset1, offset2);
}

TEST(TestTimeUtils, TestGetCurTime) {
    std::string curtime = TimeUtils::GetCurTime();
    int64_t timestamp = std::atoll(curtime.c_str());
    check_timestamp(timestamp);
}

TEST(TestTimeUtils, TestGetCurMilliTime) {
    std::string curtime = TimeUtils::GetCurMilliTime();
    int64_t timestamp = std::atoll(curtime.c_str()) / 1000;
    check_timestamp(timestamp);
}
