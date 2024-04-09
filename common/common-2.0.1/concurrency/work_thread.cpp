/// @file work_thread.cpp
/// @brief
/// @author Devin (devin@minieye.cc)

#include <stdio.h>
#include "common/base/log_level.h"
#include "common/concurrency/work_thread.h"

void WorkThread::Entry()
{
    VLOG(DEBUG_LOG) << "thread_id = " << GetId() << ", start";
    Closure<void>* task = NULL;
    while (!IsStopRequested()) {
        CheckSuspend();
        {
            MutexLocker locker(m_mutex);
            task = GetNextTask();
            if (task == NULL) {
                if (m_finish_callback != NULL) {
                    m_finish_callback->Run();
                    m_finish_callback = NULL;
                }
                m_cond.Wait(&m_mutex);
                continue;
            }
        }
        task->Run();
    }
}

void WorkThread::AddTask(Closure<void>* cls)
{
    MutexLocker locker(m_mutex);
    m_tasks.push(cls);
    m_cond.Signal();
}

Closure<void>* WorkThread::GetNextTask()
{
    if (m_tasks.size() == 0) {
        return NULL;
    }
    Closure<void>* task = m_tasks.front();
    m_tasks.pop();
    return task;
}

void WorkThread::SetFinishCallback(Closure<void>* finish_call_back)
{
    m_finish_callback = finish_call_back;
}

void WorkThread::CheckSuspend()
{
    MutexLocker locker(m_suspend_mutex);

    if (m_paused)
        m_resume_cond.Wait(m_suspend_mutex);
}

void WorkThread::Clear()
{
    MutexLocker locker(m_mutex);
    while (!m_tasks.empty()) {
        Closure<void>* task = m_tasks.front();
        delete task;
        task = NULL;
        m_tasks.pop();
    }
}

void WorkThread::Stop()
{
    Clear();
    MutexLocker locker(m_mutex);
    m_paused = false;
    SendStopRequest();
    m_resume_cond.Signal();
    m_cond.Signal();
    LOG(ERROR) << "thread_id = " << GetId() << ", stoped";
}

void WorkThread::Suspend()
{
    MutexLocker locker(m_suspend_mutex);
    m_paused = true;
    m_work_time.Stop();
    //LOG(ERROR) << "thread_id = " << GetId() << ", suspend";
}

void WorkThread::Resume()
{
    MutexLocker locker(m_suspend_mutex);
    m_paused = false;
    m_work_time.Restart();
    m_resume_cond.Signal();
    //LOG(ERROR) << "thread_id = " << GetId() << ", resume : task_num = " << m_tasks.size();
}
