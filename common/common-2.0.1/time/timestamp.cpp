#include "common/time/timestamp.h"
#include <stddef.h>

#ifdef _WIN32

#include "common/base/common_windows.h"

// Number of 100 nanosecond units from 1/1/1601 to 1/1/1970
#define EPOCH_BIAS  116444736000000000i64

int64_t GetTimeStampInUs()
{
    ULARGE_INTEGER uli;
    GetSystemTimeAsFileTime(reinterpret_cast<FILETIME*>(&uli));
    return (uli.QuadPart - EPOCH_BIAS) / 10;
}

int64_t GetTimeStampInMs()
{
    return GetTimeStampInUs() / 1000;
}

#endif

#if defined(__unix__) || defined(__APPLE__)

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/utsname.h>

/// get system time, in microseconds
int64_t SystemGetTimeStampInUs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t result = tv.tv_sec;
    result *= 1000000;
    result += tv.tv_usec;
    return result;
}

// if kernel support vsyscall64, time call will be 10 times faster.
//  check it.
int get_vsyscall64_value()
{
    int result = -1;
    int fd = open("/proc/sys/kernel/vsyscall64", O_RDONLY, 0);
    if (fd >= 0)
    {
        char buf[2];
        int num_read = read(fd, buf, 2);
        if (num_read >= 1)
        {
            switch (buf[0])
            {
            case '0':
            case '1':
            case '2':
                result = buf[0] - '0';
            }
        }
        close(fd);
    }
    return result;
}

static bool HasFastGetTimeOfDay()
{
#ifdef __x86_64__
    // For x86_64 application, as we know, gettimeofday become fast since at
    // least 2.6.32. It cost 47ms to call 1000000 times on this machine
    // (Intel(R) Xeon(R) CPU L5630  @ 2.13GHz).
    utsname u;
    if (uname(&u) < 0)
        return false;
    int major, minor, patch;
    if (sscanf(u.release, "%d.%d.%d", &major, &minor, &patch) != 3)
        return false;
    int version = major * 10000 + minor * 100 + patch;
    if (version < 20632) // 2.6.32
        return false;
    if (strcmp(u.machine, "x86_64") != 0)
        return false;
    return true;
#endif

    // But 32 bit is still not fast enough, It cost about 120ms to call
    // 1000000 times on the same above machine.
    return false;
}

static int fast_gettimeofday = HasFastGetTimeOfDay() ? 1 : -1;

int64_t GetTimeStampInUs()
{
    return SystemGetTimeStampInUs();
}

int64_t GetTimeStampInMs()
{
    int64_t timestamp = GetTimeStampInUs();
    return timestamp / 1000;
}

#endif // __unix__

