#ifndef COMMON_BASE_SINGLETON_H_INCLUDED
#define COMMON_BASE_SINGLETON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "common/base/platform_features.h"
#include "common/base/uncopyable.h"


#if HAS_THREAD_SAFE_STATICS

// namespace common {

template <typename T>
class SingletonBase
{
    DECLARE_UNCOPYABLE(SingletonBase);

private:
    struct Holder
    {
        T value;
        bool is_alive;

        explicit Holder(Holder** holder) : value(), is_alive(true)
        {
            *holder = this;
        }

        template <typename A1>
        Holder(Holder** holder, A1& a1) : value(a1), is_alive(true)
        {
            *holder = this;
        }

        ~Holder()
        {
            is_alive = false;
        }
    };

protected:
    SingletonBase() {}
    ~SingletonBase() {}

public:
    static T& Instance()
    {
        static Holder holder(&s_holder);
        if (!holder.is_alive) {
            fprintf(stderr, "Singleton has been destroyed\n");
            abort();
        }
        return holder.value;
    }

    // 获取带构造参数的单体对象时，只有第一次调用Instance的参数有效
    template <typename A1>
    static T& Instance(A1& a1)
    {
        static Holder holder(&s_holder, a1);
        if (!holder.is_alive) {
            fprintf(stderr, "Singleton has been destroyed\n");
            abort();
        }
        return holder.value;
    }

    static bool IsAlive()
    {
        return s_holder && s_holder->is_alive;
    }

private:
    static Holder* s_holder;
};

template <typename T>
typename SingletonBase<T>::Holder* SingletonBase<T>::s_holder;


// } // namespace common

#else

#include "common/concurrency/mutex.h"

// namespace common {

template <typename T>
class SingletonBase
{
    DECLARE_UNCOPYABLE(SingletonBase);

protected:
    SingletonBase() {}
    ~SingletonBase() {}

public:
    static T& Instance()
    {
        if (!m_is_alive) {
            fprintf(stderr, "Singleton has been destroyed\n");
            abort();
        }

        // double check locking optimize
        if (!m_instance)
        {
            ScopedLocker<Mutex> lock(GetSingletonLock());
            if (!m_instance)
            {
                T* p = new T();
                m_instance = p;
                atexit(Destroy);
            }
        }
        return *m_instance;
    }

    template <typename A1>
    static T& Instance(A1& a1)
    {
        if (!m_is_alive) {
            fprintf(stderr, "Singleton has been destroyed\n");
            abort();
        }

        // double check locking optimize
        if (!m_instance)
        {
            ScopedLocker<Mutex> lock(GetSingletonLock());
            if (!m_instance)
            {
                T* p = new T(a1);
                m_instance = p;
                atexit(Destroy);
            }
        }
        return *m_instance;
    }

    static bool IsAlive()
    {
        return m_instance != NULL;
    }

private:
    static void Destroy()
    {
        m_is_alive = false;
        // need not locking
        if (m_instance)
        {
            delete m_instance;
            m_instance = NULL;
        }
    }

    static Mutex& GetSingletonLock() {
#ifdef _WIN32
        static Mutex local_lock;
        return local_lock;
#else
        return m_lock;
#endif
    }

private:
    static Mutex m_lock;
    static T* volatile m_instance;
    static bool volatile m_is_alive;
};

template <typename T>
Mutex SingletonBase<T>::m_lock;

template <typename T>
T* volatile SingletonBase<T>::m_instance;

template <typename T>
bool volatile SingletonBase<T>::m_is_alive = true;

// } // namespace common

#endif

// namespace common {

template <typename T>
class Singleton : public SingletonBase<T>
{
};

// } // namespace common

#endif // COMMON_BASE_SINGLETON_H_INCLUDED
