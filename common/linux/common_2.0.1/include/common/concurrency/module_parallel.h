/// @file module_parallel.h
/// @brief
/// @author Devin (devin@minieye.cc)

#ifndef  MODULE_PARALLEL_H
#define  MODULE_PARALLEL_H
#pragma once

#include <typeinfo>
#include <iostream>
#include "common/base/log_level.h"
#include "common/base/closure.h"
#include "common/base/singleton.h"
#include "common/concurrency/condition_variable.h"
#include "common/concurrency/work_thread.h"

// ------------------------declarartion ----------------------
template <typename T>
class ModuleParallel : public SingletonBase<ModuleParallel<T> >
{
public:
    ModuleParallel();
    ~ModuleParallel();

    void Initialize(int thread_num, const std::string& name=typeid(T).name());

    void Destroy();

    /// @brief  you should reset ModuleParallel before adding new parallel func
    void Reset();

    /// @brief func for adding parallel function
    void AddParallelFunc(Closure<void>* func, int label);

    /// @brief you should wait for this func return after adding new parallel func
    void WaitForDone();

    /// @brief return thread num for parallel
    int GetThreadNum() const
    {
        return m_thread_num;
    }

private:
    bool Done();
    void FinishTask(int index);

private:
    int m_thread_num;
    Mutex m_mutex;
    ConditionVariable m_cond;
    WorkThread* m_threads;
    bool* m_done_flags;
};

// --------------------------- implementation -------------------
template <typename T>
ModuleParallel<T>::ModuleParallel() :
    m_thread_num(0),
    m_done_flags(NULL)
{
}

template <typename T>
ModuleParallel<T>::~ModuleParallel()
{
}

template <typename T>
void ModuleParallel<T>::Destroy()
{
    for (int i = 0; i < m_thread_num; ++i) {
        LOG(ERROR) << typeid(T).name() <<
            " thread " << m_threads[i].GetId() << " stop" << std::endl;
        m_threads[i].Stop();
        m_threads[i].StopAndWaitForExit();
    }

    delete[] m_threads;
    delete[] m_done_flags;
    LOG(ERROR) << typeid(T).name() << ", destroy" << ", thread_num = " << m_thread_num;
}

template <typename T>
void ModuleParallel<T>::Initialize(int thread_num, const std::string& name)
{
    MutexLocker locker(m_mutex);
    if (thread_num < 1)
        return;
    if (m_thread_num > 0)
        return;
    m_thread_num = thread_num;
    m_threads = new WorkThread[thread_num];
    m_done_flags = new bool[m_thread_num];
    for (int i = 0; i < m_thread_num; ++i) {
        m_threads[i].Start();
        m_threads[i].SetName(name);
        VLOG(DEBUG_LOG) << typeid(T).name() <<
            " thread " << m_threads[i].GetId() << " start";
    }
    VLOG(DEBUG_LOG) << typeid(T).name() << ", start" << ", thread_num = " << m_thread_num;
}

template <typename T>
void ModuleParallel<T>::Reset()
{
    for (int i = 0; i < m_thread_num; ++i) {
        m_threads[i].Suspend();    // 挂起
        m_threads[i].Clear();
        m_done_flags[i] = true;
    }
}

template <typename T>
void ModuleParallel<T>::AddParallelFunc(Closure<void>* func, int label)
{
    int index = label % m_thread_num;
    m_threads[index].AddTask(func);
    m_done_flags[index] = false;
}

template <typename T>
void ModuleParallel<T>::WaitForDone()
{
    for (int i = 0; i < m_thread_num; ++i) {
        if (!m_done_flags[i]) {
            Closure<void>* cls = NewClosure(this, &ModuleParallel<T>::FinishTask,
                    i);
            m_threads[i].SetFinishCallback(cls);
            //LOG(ERROR) << typeid(T).name() <<
                //" thread " << m_threads[i].GetId() << " start";
            m_threads[i].Resume(); // 唤醒
        }
    }
    {
        MutexLocker locker(m_mutex);
        if (!Done())
            m_cond.Wait(m_mutex);
    }
}

template <typename T>
bool ModuleParallel<T>::Done()
{
    for (int i = 0; i < m_thread_num; ++i) {
        if (!m_done_flags[i]) return false;
    }
    return true;
}

template <typename T>
void ModuleParallel<T>::FinishTask(int index)
{
    MutexLocker locker(m_mutex);
    //LOG(ERROR) << typeid(T).name() <<
        //" thread " << m_threads[index].GetId() << " Finish";
    m_done_flags[index] = true;
    if (Done())
        m_cond.Signal();
}

// -------------- macros for define module ---------------------
#define REGISTER_PARALLEL_MODULE(module_name) \
    struct module_name { \
    }; \
//    static module_name & module##_handle = ModuleParallel<module_name>::Instance();

// ------------- pre-define some module -------------------------
REGISTER_PARALLEL_MODULE(Vehicle);
REGISTER_PARALLEL_MODULE(Lane);
REGISTER_PARALLEL_MODULE(VehicleLane);
// ------------- ------------------------------------------------

#endif  // MODULE_PARALLEL_H
