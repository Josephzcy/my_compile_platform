/// Copyright (C) 2015 - MiniEye INC.

#ifdef __gnu_linux__
#include <unistd.h>
#endif

#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>

#include "common/system/system_information.h"

void rtrim(std::string* str) {
    str->erase(std::remove(str->begin(), str->end(), '\n'), str->end());
}

#ifdef __gnu_linux__
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
        result += buffer;
    }

    rtrim(&result);
    return result;
}
#endif

TEST(TestSystem, Test1) {
    unsigned int cpus = GetLogicalCpuNumber();
    ASSERT_GE(cpus, 1);
    ASSERT_LE(cpus, 1024);

#ifdef __gnu_linux__
    std::string output = exec("nproc");
    int nproc = atoi(output.c_str());
    ASSERT_EQ(cpus, nproc);
#endif
}

TEST(TestSystem, Test2) {
    uint64_t memsize = GetPhysicalMemorySize();
    ASSERT_GE(memsize, 0);

    uint64_t total_memsize = GetTotalPhysicalMemorySize();
    ASSERT_EQ(total_memsize, memsize);
}

TEST(TestSystem, Test3) {
    std::string kernerl_info;
    bool status = GetOsKernelInfo(&kernerl_info);

#ifdef __gnu_linux__
    ASSERT_TRUE(status);

    std::string kernerl_release = exec("uname -r");
    ASSERT_EQ(kernerl_info, kernerl_release);
#endif
}

TEST(TestSystem, Test4) {
    std::string arch_info;
    bool status = GetMachineArchitecture(&arch_info);

#ifdef __gnu_linux__
    ASSERT_TRUE(status);

    std::string hardware_platform = exec("uname -i");
    ASSERT_EQ(arch_info, hardware_platform);
#endif
}

TEST(TestSystem, Test5) {
    std::string username = GetUserName();

#ifdef __gnu_linux__
    std::string myname = exec("whoami");
    ASSERT_EQ(username, myname);
#endif
}
