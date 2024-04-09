/// Copyright (C) 2015 - MiniEye INC.

#include <gtest/gtest.h>
#include <ctime>
#include "common/time/stopwatch.h"

// sleep for several milliseconds
inline void msleep(int milliseconds) {
    struct timespec req;
    req.tv_sec = milliseconds / 1000;
    req.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&req, nullptr);
}

TEST(TestStopwatch, Test1) {
    Stopwatch watch;
    ASSERT_TRUE(watch.IsRunning());
    watch.Stop();
    ASSERT_FALSE(watch.IsRunning());
}

TEST(TestStopwatch, Test2) {
    Stopwatch watch(false);
    ASSERT_FALSE(watch.IsRunning());
    watch.Start();
    ASSERT_TRUE(watch.IsRunning());
    watch.Reset();
    ASSERT_FALSE(watch.IsRunning());
}

TEST(TestStopwatch, Test3) {
    Stopwatch watch;

    msleep(10);
    double secs = watch.ElapsedSeconds();
    ASSERT_GE(secs, 0.010);
    ASSERT_LE(secs, 0.015);

    msleep(10);
    int64_t msecs = watch.ElapsedMilliSeconds();
    ASSERT_GE(msecs, 20);
    ASSERT_LE(msecs, 25);

    msleep(10);
    int64_t usecs = watch.ElapsedMicroSeconds();
    ASSERT_GE(usecs, 30000);
    ASSERT_LE(usecs, 35000);

    watch.Restart();

    msleep(10);
    usecs = watch.ElapsedMicroSeconds();
    ASSERT_GE(usecs, 10000);
    ASSERT_LE(usecs, 15000);

    watch.Stop();

    usecs = watch.ElapsedMicroSeconds();
    ASSERT_GE(usecs, 10000);
    ASSERT_LE(usecs, 15000);

    watch.Reset();

    usecs = watch.ElapsedMicroSeconds();
    ASSERT_EQ(usecs, 0);
    msleep(10);
    usecs = watch.ElapsedMicroSeconds();
    ASSERT_EQ(usecs, 0);
}
