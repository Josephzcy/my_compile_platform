#include <cstddef>
#include <iostream>
#include "common/base/closure.h"
#include "common/concurrency/this_thread.h"
#include "common/concurrency/thread_pool.h"

void f1()
{
    std::cout << ThisThread::GetId() << " : f1 fun" << std::endl;
}

void f2(void* para1)
{
    std::cout << ThisThread::GetId() << " : f2 fun" << std::endl;
}

void f3(void* para1, void* para2)
{
    std::cout << ThisThread::GetId() << " : f3 fun" << std::endl;
}


int main(int argc, const char *argv[])
{
    ThreadPool pool; // 0 - cpu member
    void* null_ptr  = NULL;
    for (int i = 0; i < 1000; i++) {
        Closure<void>* cls1 = NewClosure(f1);
        pool.AddTask(cls1);

        Closure<void>* cls2 = NewClosure(f2, null_ptr);
        pool.AddTask(cls2);

        Closure<void>* cls3 = NewClosure(f3, null_ptr, null_ptr);
        pool.AddTask(cls3);
    }

    return 0;
}
