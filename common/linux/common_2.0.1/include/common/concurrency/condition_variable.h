
#ifndef COMMON_CONCURRENCY_CONDITION_VARIABLE_H
#define COMMON_CONCURRENCY_CONDITION_VARIABLE_H

#include <assert.h>

#if defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
#endif

#include "common/concurrency/mutex.h"

class ConditionVariable
{
public:
    ConditionVariable();
    ~ConditionVariable();
    void Signal();
    void Broadcast();

    // If timeout_in_ms is -1, it means infinite and equals to
    // Wait(Mutex* mutex);
    bool Wait(MutexBase* mutex, int timeout_in_ms);
    bool Wait(MutexBase& mutex, int timeout_in_ms)
    {
        return Wait(&mutex, timeout_in_ms);
    }

    void Wait(MutexBase* mutex);
    void Wait(MutexBase& mutex)
    {
        return Wait(&mutex);
    }
private:
    void CheckValid() const;
private:
#ifdef _WIN32
    void* m_hCondition;
    unsigned int m_nWaitCount;
#elif defined(__unix__) || defined(__APPLE__)
    pthread_cond_t m_hCondition;
#endif
};

#endif // COMMON_CONCURRENCY_CONDITION_VARIABLE_H

