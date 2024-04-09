#ifndef COMMON_CONCURRENCY_BASE_THREAD_H
#define COMMON_CONCURRENCY_BASE_THREAD_H

#include <string>
#include "common/concurrency/this_thread.h"
#include "common/concurrency/thread_types.h"

#undef Yield

class Thread;

// BaseThread class is designed to be used as base class,
// and override the Entry virtual method.
class BaseThread
{
    friend class Thread;

public:
    typedef ThreadHandleType HandleType;

protected:
    BaseThread();

public:
    virtual ~BaseThread();
    void SetStackSize(size_t size);

    // Must be called to start a thread, or the thread will not be created.
    // But can't be called again unless joined or detached.
    bool Start();

    // Wait for thread termination
    // the thread must be started and not detached
    bool Join();

    // Whether the SendStopRequest or StopAndWaitForExit called
    bool IsStopRequested() const;

    // Make IsStopRequested return true
    void SendStopRequest();

    // call SendStopRequest and then Join
    bool StopAndWaitForExit();

    // Whether the thread is still alive
    bool IsAlive() const;

    // Is fitable to call Join?
    bool IsJoinable() const;

    // Get handle
    HandleType GetHandle() const;

    // Get numeric thread id
    int GetId() const;

    // set name for thread
    int SetName(const std::string& name);

    static void SetDefaultStackSize(size_t size);


protected:
    // This virtual function will be called before the thread exiting
    virtual void OnExit();

private:
    // BaseThread should not support Detach, because the running thread can't
    // control the memory of BaseThread object. So if it is detached and the
    // BaseThread destructed, the running thread will access the destructed
    // object, make it private.
    bool DoDetach();

    // The derived class must override this function as the thread entry point
    virtual void Entry() = 0;

    // Do some class shared initializing
    static void Initialize();
    static void DoInitialize();
    static void Cleanup(void* param);

private:
#ifdef _WIN32
    static uint32_t s_nThreadStorageIndex;
    static unsigned int __stdcall StaticEntry(void* inThread);
#elif defined(__unix__) || defined(__APPLE__)
    static pthread_once_t s_InitializeOnce;
    static pthread_key_t  s_nMainKey;
    static void* StaticEntry(void* inThread);
#endif

private:
    static size_t s_DefaultStackSize;
    HandleType m_Handle;
    int m_Id;
    size_t m_StackSize;
    volatile bool m_bStopRequested;
    volatile bool m_IsAlive;
};

inline void BaseThread::SendStopRequest()
{
    m_bStopRequested = true;
}

inline bool BaseThread::IsStopRequested() const
{
    return m_bStopRequested;
}

inline ThreadHandleType BaseThread::GetHandle() const
{
    return m_Handle;
}

#endif // COMMON_CONCURRENCY_BASE_THREAD_H

