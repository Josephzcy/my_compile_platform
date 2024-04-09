#include "gtest/gtest.h"
#include "common/base/log_level.h"
#include "common/base/closure.h"
#include "common/base/stdint.h"
#include "common/time/time_utils.h"
#include "common/concurrency/this_thread.h"
#include "common/timer/timer_manager.h"

void TimerPrint(int64_t add_tm, int64_t trig_tm, std::string label, uint64_t timer_id)
{
    LOG(ERROR) << "timer_id " << timer_id << " " << label
        << " : real_gap = " << TimeUtils::Milliseconds() - add_tm
        << ", trig_gap = " << trig_tm
        << " triggered";
}

TEST(TM, TriggerTest)
{
    int64_t trig_tm = 100;
    TimerManager::CallbackClosure* cls = NewClosure(TimerPrint,
            TimeUtils::Milliseconds(),
            trig_tm,
            "one_time_shot");
    uint64_t one_id = TimerManager::DefaultInstance().AddOneshotTimer(trig_tm, cls);

    trig_tm = 230;
    TimerManager::CallbackClosure* per_cls = NewPermanentClosure(TimerPrint,
            TimeUtils::Milliseconds(),
            trig_tm,
            "period_time_shot");

    uint64_t period_id = TimerManager::DefaultInstance().AddPeriodTimer(trig_tm, per_cls);
    for(;;) {
        ThisThread::Sleep(1);
    }
}
