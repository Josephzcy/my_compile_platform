/// @file work_thread.h
/// @brief
/// @author Devin (devin@minieye.cc)

#ifndef  WORK_THREAD_H
#define  WORK_THREAD_H
#include <queue>
#include "common/base/closure.h"
#include "common/concurrency/base_thread.h"
#include "common/time/stopwatch.h"
#include "common/concurrency/condition_variable.h"
#include "common/concurrency/mutex.h"

class WorkThread : public BaseThread
{
public:
    WorkThread() : m_interruption_func(NULL), m_finish_callback(NULL), m_paused(false) {}
    ~WorkThread() {
        StopAndWaitForExit();
    }

    void Entry();

    void AddTask(Closure<void>* task);

    Closure<void>* GetNextTask();

    void Clear();
    void Stop();

    void ResetInter(Closure<bool>* inter)
    {
        m_interruption_func = inter;
    }

    void SetFinishCallback(Closure<void>* finish_call_back);

    void Suspend();

    void Resume();

    size_t GetTaskNum()
    {
        return m_tasks.size();
    }

private:
    void CheckSuspend();

private:
    Mutex m_mutex;
    Mutex m_suspend_mutex;
    ConditionVariable m_cond;
    ConditionVariable m_resume_cond;
    std::queue<Closure<void>* > m_tasks;
    Closure<bool>* m_interruption_func;
    Closure<void>* m_finish_callback;
    bool m_paused;
    Stopwatch m_work_time;
};

#endif  // WORK_THREAD_H
