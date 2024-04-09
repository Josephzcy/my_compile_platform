#ifndef COMMON_SYSTEM_CONCURRENCY_SPINLOCK_H
#define COMMON_SYSTEM_CONCURRENCY_SPINLOCK_H

#include <errno.h>
#include <stdlib.h>

#ifdef _WIN32

#include "common/concurrency/mutex.h"
typedef Mutex Spinlock;
#else

#include <pthread.h>

#ifdef __ANDROID__
typedef pthread_mutex_t pthread_spinlock_t;
#define pthread_spin_init pthread_mutex_init
#define pthread_spin_destroy pthread_mutex_destroy
#define pthread_spin_lock pthread_mutex_lock
#define pthread_spin_unlock pthread_mutex_unlock
#define pthread_spin_trylock pthread_mutex_trylock
#endif

#ifdef __APPLE__
#include <libkern/OSAtomic.h>
typedef OSSpinLock pthread_spinlock_t;
#endif

#include "common/concurrency/scoped_locker.h"

/// spinlock is faster than mutex at some condition, but
class Spinlock
{
public:
    typedef ScopedLocker<Spinlock> Locker;
public:
    Spinlock();
    ~Spinlock();
    void Lock();
    bool TryLock();
    void Unlock();
private:
    Spinlock(const Spinlock&);
    Spinlock& operator=(const Spinlock&);
private:
    pthread_spinlock_t m_lock;
    pid_t m_owner;
};

#endif // _WIN32

// compatible reason
typedef Spinlock::Locker SpinlockLocker;

#endif // COMMON_SYSTEM_CONCURRENCY_SPINLOCK_H

