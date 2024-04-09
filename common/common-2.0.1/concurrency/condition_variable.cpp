#include "common/concurrency/condition_variable.h"

#include <assert.h>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/time.h>
#endif

#include <stdexcept>
#include <string>

#include "common/base/check_error.h"

#ifdef _WIN32

#include "common/base/common_windows.h"
#pragma warning (disable: 4800)


ConditionVariable::ConditionVariable()
{
    m_hCondition = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_nWaitCount = 0;
    CHECK_WINDOWS_ERROR(m_hCondition != NULL);
}

ConditionVariable::~ConditionVariable()
{
    CHECK_WINDOWS_ERROR(::CloseHandle(m_hCondition));
}

void ConditionVariable::CheckValid() const
{
}

void ConditionVariable::Wait(MutexBase* mutex)
{
    // clear previous state
    CHECK_WINDOWS_ERROR(::ResetEvent(m_hCondition));
    mutex->Unlock();
    m_nWaitCount++;
    DWORD theErr = ::WaitForSingleObject(m_hCondition, INFINITE);
    m_nWaitCount--;
    mutex->Lock();
    CHECK_WINDOWS_WAIT_ERROR(theErr);
}

bool ConditionVariable::Wait(MutexBase* mutex, int timeout_in_ms)
{
    // clear previous state
    CHECK_WINDOWS_ERROR(::ResetEvent(m_hCondition));
    mutex->Unlock();
    m_nWaitCount++;
    DWORD theErr = ::WaitForSingleObject(m_hCondition, timeout_in_ms);
    m_nWaitCount--;
    mutex->Lock();

    return CHECK_WINDOWS_WAIT_ERROR(theErr);
}

void ConditionVariable::Signal()
{
    if (!::SetEvent(m_hCondition))
        throw std::runtime_error("ConditionVariable::Signal");
}

void ConditionVariable::Broadcast()
{
    // There doesn't seem like any more elegant way to
    // implement Broadcast using events in Win32.
    // This will work, it may generate spurious wakeups,
    // but condition variables are allowed to generate
    // spurious wakeups
    unsigned int waitCount = m_nWaitCount;
    for (unsigned int x = 0; x < waitCount; x++)
    {
        CHECK_WINDOWS_ERROR(::SetEvent(m_hCondition));
    }
}

// } // namespace common

#elif defined(__unix__) || defined(__APPLE__)

#include "common/time/posix_time.h"

// namespace common {

ConditionVariable::ConditionVariable()
{
    pthread_condattr_t cond_attr;
    CHECK_PTHREAD_ERROR(pthread_condattr_init(&cond_attr));
    CHECK_PTHREAD_ERROR(pthread_cond_init(&m_hCondition, &cond_attr));
    CHECK_PTHREAD_ERROR(pthread_condattr_destroy(&cond_attr));
}

ConditionVariable::~ConditionVariable()
{
    CHECK_PTHREAD_ERROR(pthread_cond_destroy(&m_hCondition));
}

void ConditionVariable::CheckValid() const
{
#ifdef __linux__
    // __total_seq will be set to -1 by pthread_cond_destroy
//    assert(m_hCondition.__data.__total_seq != -1ULL && "this cond has been destructed");
#endif
}

void ConditionVariable::Signal()
{
    CheckValid();
    CHECK_PTHREAD_ERROR(pthread_cond_signal(&m_hCondition));
}

void ConditionVariable::Broadcast()
{
    CheckValid();
    CHECK_PTHREAD_ERROR(pthread_cond_broadcast(&m_hCondition));
}

void ConditionVariable::Wait(MutexBase* mutex)
{
    CheckValid();
    CHECK_PTHREAD_ERROR(pthread_cond_wait(&m_hCondition, &mutex->m_mutex));
}

bool ConditionVariable::Wait(MutexBase* mutex, int timeout_in_ms)
{
    // -1 wait forever
    if (timeout_in_ms < 0)
    {
        Wait(mutex);
        return true;
    }

    timespec ts;
    RelativeTimeInMicoSecondsToAbsTimeInTimeSpec(timeout_in_ms*1000, &ts);
    return pthread_cond_timedwait(&m_hCondition, &mutex->m_mutex, &ts);
}

#endif

