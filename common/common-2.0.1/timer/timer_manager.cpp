#include <limits>
#include "common/base/singleton.h"
#include "common/concurrency/mutex.h"
#include "common/timer/timer_manager.h"

uint64_t TimerManager::AddTimer(
        int64_t interval, bool is_period,
        CallbackClosure* closure)
{
    assert(interval >= 0);

    MutexLocker lock(m_mutex);
    uint64_t id = NewTimerId();
    TimerEntry& timer = m_timers[id];
    timer.interval = interval;
    timer.is_period = is_period;
    timer.closure = closure;
    timer.is_enabled = true;

    SetNextTimeout(id, interval, 0);

    return id;
}

uint64_t TimerManager::AddOneshotTimer(int64_t interval, CallbackClosure* closure)
{
    assert(closure != NULL);
    assert(closure->IsSelfDelete());
    return AddTimer(interval, false, closure);
}

uint64_t TimerManager::AddPeriodTimer(int64_t interval, CallbackClosure* closure)
{
    assert(closure != NULL);
    assert(!closure->IsSelfDelete());
    return AddTimer(interval, true, closure);
}

bool TimerManager::AsyncRemoveTimerInLock(uint64_t id)
{
    TimerMap::iterator it = m_timers.find(id);
    if (it != m_timers.end()) {
        delete it->second.closure;
        m_timers.erase(it);
        return true;
    }
    return false;
}

bool TimerManager::AsyncRemoveTimer(uint64_t id)
{
    MutexLocker lock(m_mutex);
    return AsyncRemoveTimerInLock(id);
}

bool TimerManager::RemoveTimer(uint64_t id)
{
    if (id == 0) { // 0 is an invalid id.
        return false;
    }
    // to avoid deadlock, we should not wait if this function is called in
    // timer thread.
    // AsyncRemoveTimer is safe and semantic correct in this condition.
    if (ThisThread::GetId() == this->GetId())
        return AsyncRemoveTimer(id);

    for (;;) {
        {
            MutexLocker locker(m_mutex);
            if (m_running_timer != id)
                return AsyncRemoveTimerInLock(id);
        }
        // busy wait is ok for this rare case
        ThisThread::Sleep(1);
    }
}

bool TimerManager::ModifyTimer(uint64_t id, int64_t interval, CallbackClosure* closure)
{
    assert(closure != NULL);
    MutexLocker lock(m_mutex);
    TimerMap::iterator it = m_timers.find(id);
    TimerEntry* entry = FindEntry(id);
    if (entry) {
        entry->interval = interval;
        if (entry->closure != closure) { // check for safety
            delete entry->closure; // release old closure
            entry->closure = closure;
        }
        entry->revision++;
        if (entry->is_enabled) {
            SetNextTimeout(id, interval, entry->revision);
        }
        return true;
    }
    return false;
}

bool TimerManager::ModifyTimer(uint64_t id, int64_t interval)
{
    MutexLocker lock(m_mutex);
    TimerEntry* entry = FindEntry(id);
    if (entry) {
        entry->interval = interval;
        entry->revision++;
        if (entry->is_enabled) {
            SetNextTimeout(id, interval, entry->revision);
        }
        return true;
    }
    return false;
}

bool TimerManager::DisableTimer(uint64_t id)
{
    MutexLocker lock(m_mutex);
    TimerEntry* entry = FindEntry(id);
    if (entry) {
        entry->is_enabled = false;
        entry->revision++;
        return true;
    }
    return false;
}

bool TimerManager::EnableTimer(uint64_t id)
{
    MutexLocker lock(m_mutex);
    TimerEntry* entry = FindEntry(id);
    if (entry) {
        entry->is_enabled = true;
        SetNextTimeout(id, entry->interval, entry->revision);
        return true;
    }
    return false;
}

// -------------------------------------------------------
// the functions below are declared private
// mutex is not needed because they are aready under lock.
// -------------------------------------------------------
TimerManager::TimerEntry* TimerManager::FindEntry(uint64_t id)
{
    TimerMap::iterator it = m_timers.find(id);
    if (it != m_timers.end())
        return &it->second;
    return NULL;
}

bool TimerManager::GetLatestTime(int64_t* time) const
{
    if (m_timeouts.empty())
        return false;

    *time = m_timeouts.top().time;
    return true;
}

bool TimerManager::GetLatestTimeout(int64_t* time) const
{
    if (GetLatestTime(time)) {
        // convert to relative time
        int64_t now = GetCurrentTime();
        if (*time < now)
            *time = 0;
        else
            *time -= now;
        return true;
    }

    return false;
}

uint64_t TimerManager::NewTimerId()
{
    return ++m_timer_id;
}

void TimerManager::PushNextTimeout(uint64_t id, int64_t interval, uint8_t revision)
{
    Timeout timeout = { GetCurrentTime() + interval, id, revision };
    m_timeouts.push(timeout);
}

void TimerManager::SetNextTimeout(uint64_t id, int64_t interval, uint8_t revision)
{
    int64_t prev_top_time;

    if (ThisThread::GetId() == GetId()) {
        PushNextTimeout(id, interval, revision);
    } else if (GetLatestTime(&prev_top_time)) {
        PushNextTimeout(id, interval, revision);
        int64_t new_top_time = m_timeouts.top().time;
        // reschedule timer only if the new time is the most early
        if (new_top_time < prev_top_time)
            m_cond.Signal();
    } else {
        // timer manager is empty, worker thread is waiting infinitely
        PushNextTimeout(id, interval, revision);
        m_cond.Signal();
    }
}

// -------------------------------------------------------
void TimerManager::Entry()
{
    for (;;) {
        {
            MutexLocker lock(m_mutex);
            if (IsStopRequested())
                return;

            int64_t time;
            if (GetLatestTimeout(&time))
                m_cond.Wait(m_mutex, static_cast<int>(time));
            else
                m_cond.Wait(m_mutex);
        }
        Dispatch();
    }
}

bool TimerManager::Start()
{
    return BaseThread::Start();
}

void TimerManager::Stop()
{
    { // for scoped mutex lock
        MutexLocker lock(m_mutex);
        SendStopRequest();
        m_cond.Signal();
    }
    Join();
}

bool TimerManager::DequeueTimeoutEntry(uint64_t* id, TimerEntry* entry)
{
    for (;;) {
        MutexLocker locker(m_mutex);

        if (m_timeouts.empty() || m_timeouts.top().time > GetCurrentTime())
            return false;

        Timeout timeout = m_timeouts.top();
        m_timeouts.pop();

        TimerMap::iterator it = m_timers.find(timeout.timer_id);
        if (it == m_timers.end())
            continue;

        // ignore outdated timeouts
        if (!it->second.is_enabled || timeout.revision != it->second.revision)
            continue;

        *id = timeout.timer_id;
        *entry = it->second;

        if (entry->is_period) {
            // move the closure into entry temporary during running,
            // make sure closure will not be deleted in case ModifyTimer is called
            // with a difference closure
            it->second.closure = NULL;
        } else {
            m_timers.erase(it);
        }

        break;
    }

    m_running_timer = *id;
    return true;
}

void TimerManager::Dispatch()
{
    uint64_t id;
    TimerEntry entry;

    while (DequeueTimeoutEntry(&id, &entry)) {
        // run the callback in unlocked state
        entry.closure->Run(id);

        m_running_timer = 0;

        if (entry.is_period) {
            MutexLocker locker(m_mutex);
            TimerMap::iterator it = m_timers.find(id);
            if (it != m_timers.end()) {
                // restore closure if necessary
                if (entry.closure) {
                    if (it->second.closure == NULL)
                        it->second.closure = entry.closure;
                    else if (entry.closure != it->second.closure)
                        delete entry.closure; // has be assigned with a new closure
                }

                if (it->second.is_enabled) {
                    SetNextTimeout(it->first, it->second.interval,
                                   it->second.revision);
                }
            } else { // timer has already been removed.
                delete entry.closure;
            }
        }
    }
}

void TimerManager::Clear()
{
    MutexLocker locker(m_mutex);
    while (!m_timeouts.empty())
        m_timeouts.pop();

    TimerMap::iterator it;
    for (it = m_timers.begin(); it != m_timers.end(); ++it) {
        delete it->second.closure;
    }
    m_timers.clear();
}

TimerManager::~TimerManager()
{
    Stop();
    Clear();
}

void TimerManager::GetStats(TimerManager::Stats* stats)
{
    MutexLocker lock(m_mutex);
    int64_t time = 0;
    stats->oneshot_timer_num = 0;
    stats->period_timer_num = 0;
    stats->estimate_runover_time = 0;
    TimerMap::const_iterator it;
    for (it = m_timers.begin(); it != m_timers.end(); ++it) {
        if (it->second.is_period) {
            stats->period_timer_num++;
            time = std::numeric_limits<uint64_t>::max();
        } else {
            if (it->second.interval > time)
                time = it->second.interval;
            stats->oneshot_timer_num++;
        }
    }
    stats->estimate_runover_time = time;
}

TimerManager& TimerManager::DefaultInstance()
{
    return Singleton<TimerManager>::Instance();
}

void TimerManager::SetThreadName(const std::string& thread_name)
{
    SetName(thread_name);
}
