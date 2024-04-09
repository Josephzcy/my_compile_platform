#include "common/base/check_error.h"
#include "common/concurrency/spinlock.h"
#include "common/concurrency/this_thread.h"

#ifndef _WIN32

#ifdef __APPLE__

Spinlock::Spinlock() : m_lock(0) {
    m_owner = 0;
}

Spinlock::~Spinlock() {
    m_owner = -1;
}

void Spinlock::Lock() {
    OSSpinLockLock(&m_lock);
    m_owner = ThisThread::GetId();
}

bool Spinlock::TryLock() {
    if (OSSpinLockTry(&m_lock)) {
        m_owner = ThisThread::GetId();
        return true;
    }
    return false;
}

void Spinlock::Unlock() {
    m_owner = 0;
    OSSpinLockUnlock(&m_lock);
}

#else


Spinlock::Spinlock()
{
    CHECK_PTHREAD_ERROR(pthread_spin_init(&m_lock, 0));
    m_owner = 0;
}

Spinlock::~Spinlock()
{
    CHECK_PTHREAD_ERROR(pthread_spin_destroy(&m_lock));
    m_owner = -1;
}

void Spinlock::Lock()
{
    CHECK_PTHREAD_ERROR(pthread_spin_lock(&m_lock));
    m_owner = ThisThread::GetId();
}

bool Spinlock::TryLock()
{
    if (CHECK_PTHREAD_TRYLOCK_ERROR(pthread_spin_trylock(&m_lock)))
    {
        m_owner = ThisThread::GetId();
        return true;
    }
    return false;
}

void Spinlock::Unlock()
{
    m_owner = 0;
    CHECK_PTHREAD_ERROR(pthread_spin_unlock(&m_lock));
}
#endif

#endif
