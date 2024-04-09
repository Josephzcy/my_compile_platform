#include "common/memory/mempool.h"
#include "common/memory/mempool_impl.h"

MemPool* s_pMemPool = NULL;

/// 初始化MemPool
bool MemPool_Initialize(size_t dwMaxMemSize)
{
    if (s_pMemPool == NULL)
        s_pMemPool = new MemPool(dwMaxMemSize);

    return true;
}

/// 释放MemPool
void MemPool_Destroy()
{
    delete s_pMemPool;
    s_pMemPool = NULL;
}

/// 线程安全的分配内存资源
void* MemPool_Allocate(size_t dwSize)
{
    return s_pMemPool->Allocate(dwSize);
}

void MemPool_Free(void* p)
{
    s_pMemPool->Free(p);
}

size_t MemPool_GetAllocatedSize()
{
    return s_pMemPool->GetAllocatedSize();
}

size_t MemPool_GetPooledSize()
{
    return s_pMemPool->GetPooledSize();
}

size_t MemPool_GetBlockSize(const void* ptr)
{
    return s_pMemPool->GetBlockSize(ptr);
}

bool MemPool_IsValid(void* ptr)
{
    return s_pMemPool->IsValid(ptr);
}

