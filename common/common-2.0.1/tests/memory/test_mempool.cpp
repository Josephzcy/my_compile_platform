/// Copyright (C) 2015 - MiniEye INC.

#include <gtest/gtest.h>
#include <iostream>
#include "common/memory/mempool.h"

TEST(TestMemPool, Test1) {
    bool res = MemPool_Initialize();
    ASSERT_EQ(res, true);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 0);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    void* ptr = MemPool_Allocate(200);
    ASSERT_EQ(MemPool_GetBlockSize(ptr), 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    MemPool_Free(ptr);  // return memory to the pool

    ASSERT_EQ(MemPool_GetAllocatedSize(), 0);
    ASSERT_EQ(MemPool_GetPooledSize(), 1024);

    MemPool_Destroy();
}

TEST(TestMemPool, Test2) {
    bool res = MemPool_Initialize();
    ASSERT_EQ(res, true);

    void* ptr1 = MemPool_Allocate(200);
    ASSERT_EQ(MemPool_GetBlockSize(ptr1), 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    void* ptr2 = MemPool_Allocate(200);
    ASSERT_EQ(MemPool_GetBlockSize(ptr2), 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 2048);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    void* ptr3 = MemPool_Allocate(200);
    ASSERT_EQ(MemPool_GetBlockSize(ptr3), 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 3072);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    MemPool_Free(ptr1);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 2048);
    ASSERT_EQ(MemPool_GetPooledSize(), 1024);

    MemPool_Free(ptr2);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 2048);

    MemPool_Free(ptr3);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 0);
    ASSERT_EQ(MemPool_GetPooledSize(), 3072);

    void* ptr4 = MemPool_Allocate(200);
    ASSERT_EQ(MemPool_GetBlockSize(ptr4), 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 2048);

    void* ptr5 = MemPool_Allocate(2000);
    ASSERT_EQ(MemPool_GetBlockSize(ptr5), 2048);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 3072);
    ASSERT_EQ(MemPool_GetPooledSize(), 2048);

    MemPool_Free(ptr4);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 2048);
    ASSERT_EQ(MemPool_GetPooledSize(), 3072);

    MemPool_Free(ptr5);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 0);
    ASSERT_EQ(MemPool_GetPooledSize(), 5120);

    MemPool_Destroy();
}

TEST(TestMemPool, Test3) {
    bool res = MemPool_Initialize();
    ASSERT_EQ(res, true);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 0);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    void* ptr1 = MemPool_Allocate(20 * 2048 * 1024);
    ASSERT_EQ(MemPool_GetBlockSize(ptr1), 20 * 2048 * 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 20 * 2048 * 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    void* ptr2 = MemPool_Allocate(10 * 2048 * 1024);
    ASSERT_EQ(MemPool_GetBlockSize(ptr2), 16 * 2048 * 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 36 * 2048 * 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    void* ptr3 = MemPool_Allocate(10 * 2048 * 1024);
    ASSERT_EQ(MemPool_GetBlockSize(ptr3), 16 * 2048 * 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 52 * 2048 * 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    MemPool_Free(ptr1);  // return memory to system

    ASSERT_EQ(MemPool_GetAllocatedSize(), 32 * 2048 * 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 0);

    MemPool_Free(ptr2);  // return memory to the pool

    ASSERT_EQ(MemPool_GetAllocatedSize(), 16 * 2048 * 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 16 * 2048 * 1024);

    MemPool_Free(ptr3);  // return memory to the pool

    ASSERT_EQ(MemPool_GetAllocatedSize(), 0);
    ASSERT_EQ(MemPool_GetPooledSize(), 32 * 2048 * 1024);

    void* ptr4 = MemPool_Allocate(200);
    ASSERT_EQ(MemPool_GetBlockSize(ptr4), 1024);

    ASSERT_EQ(MemPool_GetAllocatedSize(), 1024);
    ASSERT_EQ(MemPool_GetPooledSize(), 32 * 2048 * 1024);

    MemPool_Free(ptr4);  // return memory to the pool

    ASSERT_EQ(MemPool_GetAllocatedSize(), 0);
    ASSERT_EQ(MemPool_GetPooledSize(), 32 * 2048 * 1024 + 1024);

    MemPool_Destroy();
}
