#ifndef COMMON_SYSTEM_CHECK_ERROR_H
#define COMMON_SYSTEM_CHECK_ERROR_H
#pragma once

#ifdef _WIN32
#include "common/base/common_windows.h"
#include <stdlib.h>
namespace common {

    static void ReportError(const char* function_name, DWORD error_code);


    void CheckWindowsError(const char* function_name, bool error);

    bool CheckWindowsWaitError(const char* function_name, unsigned int code);

    void CheckNtError(const char* function_name, long ntstatus); // NOLINT(runtime/int)
} // namespace common

#define CHECK_WINDOWS_ERROR(bool_value) \
    ::common::CheckWindowsError(__FUNCTION__, bool_value)

#define CHECK_WINDOWS_WAIT_ERROR(expr) \
    ::common::CheckWindowsWaitError(__FUNCTION__, expr)

#define CHECK_NT_ERROR(expr) \
    ::common::CheckNtError(__FUNCTION__, expr)

#define CHECK_ERRNO_ERROR(expr) { \
    printf("CHECK_ERRNO_ERROR not implement on win32, expr : %s\n ", expr); \
}

#endif

#if defined(__unix__) || defined(__APPLE__)


// namespace common {

void HandleErrnoError(const char* function_name, int error);
inline void CheckErrnoError(const char* function_name, int error)
{
    if (error)
        HandleErrnoError(function_name, error);
}

void HandlePosixError(const char* function_name);
inline void CheckPosixError(const char* function_name, int result)
{
    if (result < 0)
        HandlePosixError(function_name);
}

bool HandlePosixTimedError(const char* function_name);
inline bool CheckPosixTimedError(const char* function_name, int result)
{
    if (result < 0)
        return HandlePosixTimedError(function_name);
    return true;
}

bool HandlePthreadTimedError(const char* function_name, int error);
inline bool CheckPthreadTimedError(const char* function_name, int error)
{
    if (error)
        return HandlePthreadTimedError(function_name, error);
    return true;
}

bool HandlePthreadTryLockError(const char* function_name, int error);
inline bool CheckPthreadTryLockError(const char* function_name, int error)
{
    if (error)
        return HandlePthreadTryLockError(function_name, error);
    return true;
}

#define CHECK_ERRNO_ERROR(expr) \
    CheckErrnoError(__PRETTY_FUNCTION__, (expr))

#define CHECK_POSIX_ERROR(expr) \
    CheckPosixError(__PRETTY_FUNCTION__, (expr))

#define CHECK_POSIX_TIMED_ERROR(expr) \
    CheckPosixTimedError(__PRETTY_FUNCTION__, (expr))

#define CHECK_PTHREAD_ERROR(expr) \
    CHECK_ERRNO_ERROR((expr))

#define CHECK_PTHREAD_TIMED_ERROR(expr) \
    CheckPthreadTimedError(__PRETTY_FUNCTION__, (expr))

#define CHECK_PTHREAD_TRYLOCK_ERROR(expr) \
    CheckPthreadTryLockError(__PRETTY_FUNCTION__, (expr))

// } // namespace common

#endif

#endif // COMMON_SYSTEM_CHECK_ERROR_H
