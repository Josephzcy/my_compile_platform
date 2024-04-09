#include "common/concurrency/this_thread.h"

#include <string.h>

#if __unix__
#include <pthread.h>
#ifdef ANDROID
#include <sys/types.h>
#elif defined(__APPLE__)
#include <sys/syscall.h>
#else
#include <syscall.h>
#endif
#include <unistd.h>

namespace {

static bool SupportTls() __attribute__((pure));

#if defined(__linux__) && defined(__i386__) // All 64 bit machines are new.
// Slackware/2.4.30 kernel, using linuxthreads, doesn't really support __thread keyword,
// will generate runtime error
static bool SupportTls()
{
    char buffer[64];
    confstr(_CS_GNU_LIBPTHREAD_VERSION, buffer, sizeof(buffer));
    return strstr(buffer, "NPTL") != NULL;
}
#else
static inline bool SupportTls()
{
    return true;
}
#endif
// If symbol is weak, its address may be NULL. But the optimizer may always
// think it's non-NULL, so simply compare its address to NULL maybe not work.
// For __thread variable, in some unknown case, the address may also be NULL.
template <typename T>
static inline bool SymbolExists(const T& value)
{
    const T* volatile address = &value;
    return address != NULL;
}

} // namespace

ThreadHandleType ThisThread::GetHandle()
{
    return ::pthread_self();
}

int ThisThread::GetId()
{
#ifdef ANDROID
    return gettid();
#else
    static const bool support_tls = SupportTls();
    if (support_tls)
    {
        static __thread pid_t tid;
        if (!SymbolExists(tid))
            return syscall(SYS_gettid);
        if (tid == 0)
            tid = syscall(SYS_gettid);
        return tid;
    }
    return syscall(SYS_gettid);
#endif
}

void ThisThread::Exit()
{
    pthread_exit(NULL);
}

void ThisThread::Sleep(int64_t time_in_ms)
{
    if (time_in_ms >= 0)
    {
        timespec ts = { time_in_ms / 1000, (time_in_ms % 1000) * 1000000 };
        nanosleep(&ts, &ts);
    }
    else
    {
        pause();
    }
}

void ThisThread::Yield()
{
    sched_yield();
}

int ThisThread::GetLastErrorCode()
{
    return errno;
}

bool ThisThread::IsMain()
{
    return ThisThread::GetId() == getpid();
}

int ThisThread::SetName(const std::string& name)
{
    int rc = pthread_setname_np(GetHandle(), name.c_str());
    return rc;
}

// } // namespace common

#endif // __unix__

#ifdef _WIN32
#include <process.h>
#include <winsock2.h>
#include "common/base/common_windows.h"
#undef Yield
#pragma warning (disable: 4244)
// namespace common {

ThreadHandleType ThisThread::GetHandle()
{
    return ::GetCurrentThread();
}

int ThisThread::GetId()
{
    return ::GetCurrentThreadId();
}

void ThisThread::Sleep(int64_t time_in_ms)
{
    ::Sleep(time_in_ms);
}

void ThisThread::Exit()
{
    ::_endthreadex(0);
}

void ThisThread::Yield()
{
    ::SwitchToThread();
}

int ThisThread::GetLastErrorCode()
{
    unsigned int error = ::WSAGetLastError();
    return WindowsErrorToPosixErrno(error);
}

static int s_main_thread_id = ThisThread::GetId();

bool ThisThread::IsMain()
{
    if (s_main_thread_id != 0)
    {
        return GetId() != s_main_thread_id;
    }

    // Program should not create threade before main,
    // If global initialization is not ran, assume in main.
    return true;
}


#pragma comment(lib, "ws2_32")

#endif // _WIN32

