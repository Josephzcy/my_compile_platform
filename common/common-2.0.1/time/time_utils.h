
#ifndef COMMON_TIME_TIME_UTILS_H
#define COMMON_TIME_TIME_UTILS_H
#pragma once

#include "common/base/stdint.h"
#include <string>

struct TimeUtils
{
    // Milliseconds always returns milliseconds(1/1000s) since Jan 1, 1970 GMT.
    static int64_t Milliseconds();

    // Milliseconds always returns microseconds(1/1000000s) since Jan 1, 1970 GMT.
    static int64_t Microseconds();

    // Returns the offset in hours between local time and GMT (or UTC) time.
    static int GetGMTOffset();

    static std::string GetCurTime();

    static std::string GetCurMilliTime();
};

#endif // COMMON_TIME_TIME_UTILS_H

