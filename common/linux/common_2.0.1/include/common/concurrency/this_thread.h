#ifndef COMMON_SYSTEM_CONCURRENCY_THIS_THREAD_H
#define COMMON_SYSTEM_CONCURRENCY_THIS_THREAD_H
#pragma once
#include <string>
#include "common/base/stdint.h"
#include "common/concurrency/thread_types.h"
#ifdef _WIN32
#undef Yield
#endif

/// thread scoped attribute and operations of current thread
class ThisThread
{
    ThisThread();
    ~ThisThread();
public:
    static void Exit();
    static void Yield();
    static void Sleep(int64_t time_in_ms);
    static int GetLastErrorCode();
    static ThreadHandleType GetHandle();
    static int GetId();
    static bool IsMain();
    static int SetName(const std::string& name);
};

#endif // COMMON_SYSTEM_CONCURRENCY_THIS_THREAD_H
