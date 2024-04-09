#include "common/concurrency/thread_types.h"

ThreadAttribute& ThreadAttribute::SetName(const std::string& name)
{
    m_name = name;
    return *this;
}

#if defined(__unix__) || defined(__APPLE__)
ThreadAttribute::ThreadAttribute()
{
    pthread_attr_init(&m_attr);
}

ThreadAttribute::~ThreadAttribute()
{
    pthread_attr_destroy(&m_attr);
}

ThreadAttribute& ThreadAttribute::SetStackSize(size_t size)
{
    pthread_attr_setstacksize(&m_attr, size);
    return *this;
}

ThreadAttribute& ThreadAttribute::SetDetached(bool detached)
{
    pthread_attr_setdetachstate(&m_attr, detached);
    return *this;
}

ThreadAttribute& ThreadAttribute::SetPriority(int priority)
{
    return *this;
}
#endif // __unix__

#ifdef _WIN32

#endif

