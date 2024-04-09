#ifndef COMMON_CONCURRENCY_RWLOCK_H
#define COMMON_CONCURRENCY_RWLOCK_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <string>

#include "common/concurrency/scoped_locker.h"
#include "common/base/check_error.h"

#if defined(__unix__) || defined(__APPLE__)

#include <pthread.h>

/// Reader/Writer lock
class RWLock
{
public:
    typedef ScopedReaderLocker<RWLock> ReaderLocker;
    typedef ScopedWriterLocker<RWLock> WriterLocker;

    enum Kind {
#if defined(ANDROID)
#else
        kKindPreferReader = PTHREAD_RWLOCK_PREFER_READER_NP,

        // Setting the value read-write lock kind to PTHREAD_RWLOCK_PREFER_WRITER_NP,
        // results in the same behavior as setting the value to PTHREAD_RWLOCK_PREFER_READER_NP.
        // As long as a reader thread holds the lock the thread holding a write lock will be
        // starved. Setting the kind value to PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP,
        // allows the writer to run. However, the writer may not be recursive as is implied by the
        // name.
        kKindPreferWriter = PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP,
        kKindDefault = PTHREAD_RWLOCK_DEFAULT_NP,
#endif
    };

public:
    RWLock()
    {
        // note: default rwlock is prefer reader
        CHECK_PTHREAD_ERROR(pthread_rwlock_init(&m_lock, NULL));
    }

    explicit RWLock(Kind kind)
    {
        pthread_rwlockattr_t attr;
        CHECK_PTHREAD_ERROR(pthread_rwlockattr_init(&attr));
#if defined(ANDROID)
        CHECK_PTHREAD_ERROR(pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE));
#else
        CHECK_PTHREAD_ERROR(pthread_rwlockattr_setkind_np(&attr, kind));
#endif
        CHECK_PTHREAD_ERROR(pthread_rwlock_init(&m_lock, &attr));
        CHECK_PTHREAD_ERROR(pthread_rwlockattr_destroy(&attr));
    }

    ~RWLock()
    {
        CHECK_PTHREAD_ERROR(pthread_rwlock_destroy(&m_lock));
    }

    void ReaderLock()
    {
        CHECK_PTHREAD_ERROR(pthread_rwlock_rdlock(&m_lock));
    }

    bool TryReaderLock()
    {
        return CHECK_PTHREAD_TRYLOCK_ERROR(pthread_rwlock_tryrdlock(&m_lock));
    }

    void WriterLock()
    {
        CHECK_PTHREAD_ERROR(pthread_rwlock_wrlock(&m_lock));
    }

    bool TryWriterLock()
    {
        return CHECK_PTHREAD_TRYLOCK_ERROR(pthread_rwlock_trywrlock(&m_lock));
    }

    void Unlock()
    {
        CHECK_PTHREAD_ERROR(pthread_rwlock_unlock(&m_lock));
    }

    // names for scoped lockers
    void ReaderUnlock()
    {
        Unlock();
    }

    void WriterUnlock()
    {
        Unlock();
    }

public: // only for test and debug, should not be used in normal code logical
    bool IsReaderLocked() const
    {
        // accessing pthread private data: nonportable but no other way
#if !defined(ANDROID)
        return m_lock.__data.__nr_readers != 0;
#else
        return m_lock.state > 0;
#endif
    }

    bool IsWriterLocked() const
    {
#if !defined(ANDROID)
        return m_lock.__data.__writer != 0;
#else
        return m_lock.state < 0;
#endif
    }

    bool IsLocked() const
    {
        return IsReaderLocked() || IsWriterLocked();
    }

private: // forbid copying
    RWLock(const RWLock& src);
    RWLock& operator=(const RWLock& rhs);
private:
    pthread_rwlock_t m_lock;
};

#elif defined _WIN32

#include "common/concurrency/mutex.h"

// fallback to mutex
class RWLock
{
public:
    typedef ScopedReaderLocker<RWLock> ReaderLocker;
    typedef ScopedWriterLocker<RWLock> WriterLocker;
public:
    RWLock() : m_lock_count(0)
    {
    }

    void ReaderLock()
    {
        m_mutex.Lock();
        ++m_lock_count;
    }

    bool TryReaderLock()
    {
        bool ret = m_mutex.TryLock();
        if (ret)
            ++m_lock_count;
        return ret;
    }

    void WriterLock()
    {
        m_mutex.Lock();
        --m_lock_count;
    }

    bool TryWriterLock()
    {
        bool ret = m_mutex.TryLock();
        if (ret)
            ++m_lock_count;
        return ret;
    }

    void Unlock()
    {
        m_lock_count = 0;
        m_mutex.Unlock();
    }

    void ReaderUnlock()
    {
        Unlock();
    }

    void WriterUnlock()
    {
        Unlock();
    }

public: // only for test and debug, should not be used in normal code logical
    bool IsReaderLocked() const
    {
        return m_lock_count > 0;
    }

    bool IsWriterLocked() const
    {
        return m_lock_count < 0;
    }

    bool IsLocked() const
    {
        return m_lock_count != 0;
    }

private:
    // mutex is noncopyable, so copy ctor and assign can't be generated
    Mutex m_mutex;

    // > 0: reader locked
    // < 0: writer locker
    // = 0: unlocked
    int m_lock_count;
};


#endif

#endif // COMMON_CONCURRENCY_RWLOCK_H
