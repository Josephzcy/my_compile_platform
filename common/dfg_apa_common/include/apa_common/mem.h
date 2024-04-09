// Copyright [2019] - MiniEye INC.
#pragma once
#if defined(USE_MEM_POOL)
#include "common/memory/mempool.h"
#endif

#include <string.h>
#include <stdint.h>

template <typename T>
T * Allocate(uint32_t num) {
    T* buff = NULL;
    uint32_t byte_size = sizeof(T) * num;
#if defined(USE_MEM_POOL)
    buff = MemPool_Allocate(byte_size);
#else
    buff = new T[num];
#endif
    memset(buff, 0, byte_size);
    return buff;
}

template <typename T>
void Free(T* ptr) {
    if (ptr == NULL)
        return;
#if defined(USE_MEM_POOL)
    MemPool_Free(ptr);
#else
    delete []ptr;
#endif
}
