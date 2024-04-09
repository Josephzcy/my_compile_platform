#ifndef  MEMPOOL_H
#define  MEMPOOL_H

#include "common/base/stdint.h"

//////////////////////////////////////////////////////////////////////////
// 初始化MemPool;
bool MemPool_Initialize(size_t dwMaxMemSize = 16 * 2048 * 1024);

//////////////////////////////////////////////////////////////////////////
// 释放MemPool;
void MemPool_Destroy();

//////////////////////////////////////////////////////////////////////////
// 线程安全的分配内存资源
void* MemPool_Allocate(size_t dwSize);
void MemPool_Free(void* p);

size_t MemPool_GetAllocatedSize();
size_t MemPool_GetPooledSize();

size_t MemPool_GetBlockSize(const void* ptr);

bool MemPool_IsValid(void* ptr);

#endif  // MEMPOOL_H
