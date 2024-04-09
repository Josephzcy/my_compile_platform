#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/base/check_error.h"

#ifdef _WIN32
// DO NOTHING
namespace common {
    static void ReportError(const char* function_name, DWORD error_code)
    {
        abort();
    }

    void CheckWindowsError(const char* function_name, bool error)
    {
        if (!error)
            ReportError(function_name, ::GetLastError());
    }

    bool CheckWindowsWaitError(const char* function_name, unsigned int code)
    {
        switch (code)
        {
        case WAIT_TIMEOUT:
            return false;
        case WAIT_FAILED:
            ReportError(function_name, ::GetLastError());
        }
        return true;
    }

    void CheckNtError(const char* function_name, long ntstatus) // NOLINT(runtime/int)
    {
        if (ntstatus)
            ReportError(function_name, ntstatus);
    }

} // namespace
#endif

#if defined(__unix__) || defined(__APPLE__)

// namespace common {

void HandleErrnoError(const char* function_name, int error)
{
    const char* msg = strerror(error);
    fprintf(stderr, "%s: Fatal error, %s", function_name, msg);
    abort();
}

void HandlePosixError(const char* function_name)
{
    HandleErrnoError(function_name, errno);
}

bool HandlePosixTimedError(const char* function_name)
{
    int error = errno;
    if (error == ETIMEDOUT)
        return false;
    HandleErrnoError(function_name, error);
    return true;
}

bool HandlePthreadTimedError(const char* function_name, int error)
{
    if (error == ETIMEDOUT)
        return false;
    HandleErrnoError(function_name, error);
    return false;
}

bool HandlePthreadTryLockError(const char* function_name, int error)
{
    if (error == EBUSY || error == EAGAIN)
        return false;
    HandleErrnoError(function_name, error);
    return false;
}

#endif // __unix__
