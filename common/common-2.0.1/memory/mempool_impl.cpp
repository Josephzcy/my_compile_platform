#include <algorithm>
#include "common/memory/mempool_impl.h"
#include "common/base/log_level.h"
#include "common/base/array_size.h"

struct MemPool::BLOCK_HEADER
{
    unsigned char Magic[2]; // magic number to check, must be 'MP'
    short Index;            // 内存单元索引，-1 表示不在 cache 里
    unsigned int Size;      // 单元的大小
};


static const size_t  s_wSizeArray[] =
{
    MIN_MEMUNIT_SIZE,
    2 * MIN_MEMUNIT_SIZE,
    4 * MIN_MEMUNIT_SIZE,
    8 * MIN_MEMUNIT_SIZE,
    16 * MIN_MEMUNIT_SIZE,
    32 * MIN_MEMUNIT_SIZE,
    64 * MIN_MEMUNIT_SIZE,
    128 * MIN_MEMUNIT_SIZE,
    256 * MIN_MEMUNIT_SIZE,
    512 * MIN_MEMUNIT_SIZE,
    1024 * MIN_MEMUNIT_SIZE,
    2048 * MIN_MEMUNIT_SIZE,
    2 * 2048 * MIN_MEMUNIT_SIZE,
    4 * 2048 * MIN_MEMUNIT_SIZE,
    8 * 2048 * MIN_MEMUNIT_SIZE,
    16 * 2048 * MIN_MEMUNIT_SIZE
};

static const size_t s_wDefaultInitialCount[] =
{
    5000, // 1024,
    5000, // 2048,
    5000, // 4096,
    500, // 8192
    50, // 1024,
    50, // 1024,
    50, // 128,
    50,
    50,
    20,
    20,
    20,
    20,
    20,
    20,
    20
};

MemPool::MemPool(size_t dwMaxMemSize):
    m_poolMutex(),
    m_dwQueueNum(MAX_INDEX_NUM),
    m_allocated_memory_size(0),
    m_pooled_memory_size(0),
    m_dwAllocateCount(0),
    m_dwFreeCount(0)
{
    LOG_IF(FATAL, (dwMaxMemSize < MIN_MEMUNIT_SIZE)) << "unexpected";
    LOG_IF(FATAL, ((dwMaxMemSize % MIN_MEMUNIT_SIZE) != 0)) << "unexpected";
    LOG_IF(FATAL, (dwMaxMemSize > MAX_MEMUNIT_SIZE)) << "unexpected";
}

MemPool::~MemPool()
{
    //////////////////////////////////////////////////////////////////////////
    // 释放MemPool中的内存;
    BLOCK_HEADER* pBlock = NULL;

    for (unsigned int i = 0; i < m_dwQueueNum; i++)
    {
        while (!(m_memPool[i].empty()))
        {
            pBlock = m_memPool[i].front();

            if (pBlock != NULL)
            {
                FreeBlock(pBlock);
            }

            m_memPool[i].pop_front();
        }
    }
}

bool MemPool::IsValidMemoryBlock(const MemPool::BLOCK_HEADER* block_header)
{
    if (block_header->Magic[0] != 'M' || block_header->Magic[1] != 'P')
        return false;

    if (block_header->Index == -1)
        return block_header->Size > MAX_MEMUNIT_SIZE;

    return block_header->Index < MAX_INDEX_NUM &&
        block_header->Size <= MAX_MEMUNIT_SIZE;
}

MemPool::BLOCK_HEADER* MemPool::AllocateBlock(unsigned int index)
{
    size_t size = sizeof(BLOCK_HEADER) + s_wSizeArray[index];
    BLOCK_HEADER* p = static_cast<BLOCK_HEADER*>(operator new(size));
    p->Magic[0] = 'M';
    p->Magic[1] = 'P';
    p->Index = index;
    p->Size = static_cast<unsigned int>(s_wSizeArray[index]);
    return p;
}

MemPool::BLOCK_HEADER* MemPool::AllocateLargeBlock(size_t size)
{
    size_t new_size = sizeof(BLOCK_HEADER) + size;
    BLOCK_HEADER* p = static_cast<BLOCK_HEADER*>(operator new(new_size));
    p->Magic[0] = 'M';
    p->Magic[1] = 'P';
    p->Index = -1;
    p->Size = static_cast<unsigned int>(size);
    return p;
}

void MemPool::FreeBlock(BLOCK_HEADER* block)
{
    operator delete(block);
}

//////////////////////////////////////////////////////////////////////////
// 功能描述:             分配一个大小为dwSize的MemUnit
// 输入参数:             @参数1: dwSize; MemUnit的大小;
// 返回值:               MemUnit的指针;
void* MemPool::Allocate(size_t dwSize)
{
    Spinlock::Locker locker(&m_poolMutex);
    BLOCK_HEADER* pBlock = NULL;
    if (dwSize != 0)
    {
        int dwArrayIndex = GetUnitIndex(dwSize);

        if (dwArrayIndex >= MAX_INDEX_NUM)
        {
            pBlock = AllocateLargeBlock(dwSize);
        }
        else
        {
            {

                if (!m_memPool[dwArrayIndex].empty())
                {
                    m_pooled_memory_size -= s_wSizeArray[dwArrayIndex];
                    pBlock = m_memPool[dwArrayIndex].front();
                    m_memPool[dwArrayIndex].pop_front();
                }
            }

            if (!pBlock)
            {
                pBlock = AllocateBlock(dwArrayIndex);
            }
        }
    }

    if (pBlock)
    {
        m_dwAllocateCount++;
        m_allocated_memory_size += pBlock->Size;
        return pBlock + 1;
     }

    return NULL;
}

void* MemPool::AlignedAllocate(size_t dwSize, size_t alignment)
{
    Spinlock::Locker locker(&m_poolMutex);
    size_t ptr_size = sizeof(void*);
    size_t a = alignment - 1;
    size_t raw_size = dwSize + a + ptr_size;

    int dwArrayIndex = GetUnitIndex(raw_size);

    BLOCK_HEADER* pBlock = NULL;
    if (dwArrayIndex >= MAX_INDEX_NUM) {
        pBlock = AllocateLargeBlock(raw_size);
    } else {
        {

            if (!m_memPool[dwArrayIndex].empty())
            {
                m_pooled_memory_size -= s_wSizeArray[dwArrayIndex];
                pBlock = m_memPool[dwArrayIndex].front();
                m_memPool[dwArrayIndex].pop_front();
            }
        }

        if (!pBlock)
        {
            pBlock = AllocateBlock(dwArrayIndex);
        }
    }
    if (pBlock) {
        void* raw = (void*)(pBlock + 1);
        void* aligned = (void*)(((size_t)raw + ptr_size + a) & ~a);
        *(void**)((size_t)aligned - ptr_size) = raw;
        {
            m_dwAllocateCount++;
            m_allocated_memory_size += pBlock->Size;
        }
        return aligned;
    }
    return NULL;
}


// 功能描述:                释放一个MemUnit的资源
// 输入参数:                @参数1: pUnit, 内存块的指针;
// 返回值:              无
void MemPool::Free(void* p)
{
    Spinlock::Locker locker(&m_poolMutex);
    if (p)
    {
        BLOCK_HEADER* pBlock = reinterpret_cast<BLOCK_HEADER*>(p) - 1;
        LOG_IF(FATAL, !(IsValidMemoryBlock(pBlock))) << "unexpected : ptr = " << std::hex << pBlock;

        int dwArrayIndex = pBlock->Index;
        size_t size = pBlock->Size;

        if (dwArrayIndex == -1)
        {
            LOG(ERROR) << "Free block. size = " << size;
            FreeBlock(pBlock);
        }
        else
        {
            {
                size_t dwCurQueueSize = m_memPool[dwArrayIndex].size();
                if (dwCurQueueSize < s_wDefaultInitialCount[dwArrayIndex])
                {
                    m_memPool[dwArrayIndex].push_back(pBlock);
                    m_pooled_memory_size += s_wSizeArray[dwArrayIndex];
                    pBlock = NULL;
                }
            }

            if (pBlock)
            {
//                LOG(ERROR) << "Free block. size = " << size;
                FreeBlock(pBlock);
            }
        }

        {
            ++m_dwFreeCount;
            m_allocated_memory_size -= size;
        }
    }
}

void MemPool::AlignedFree(void* aligned)
{
    if (!aligned) return;
    void* raw = *(void**)((char*)aligned - sizeof(void*));
    Free(raw);
}

bool MemPool::IsValid(void* p)
{
    BLOCK_HEADER* pBlock = reinterpret_cast<BLOCK_HEADER*>(p) - 1;

    return IsValidMemoryBlock(pBlock);
}


size_t MemPool::GetBlockSize(const void* ptr) const
{
    if (ptr)
    {
        const BLOCK_HEADER* pBlock = reinterpret_cast<const BLOCK_HEADER*>(ptr) - 1;
        return pBlock->Size;
    }
    return 0;
}

size_t MemPool::GetAllocatedSize() const
{
    return m_allocated_memory_size;
}

size_t MemPool::GetPooledSize() const
{
    return m_pooled_memory_size;
}

// 功能描述:                获取内存块的循环Buffer索引编号;
// 输入参数:                @参数1: dwSize, 内存块的大小;
// 返回值:              索引编号值;
int MemPool::GetUnitIndex(size_t dwSize)
{
    const size_t* p = std::lower_bound(s_wSizeArray, s_wSizeArray + ARRAY_SIZE(s_wSizeArray), dwSize);
    return static_cast<int>(p - s_wSizeArray);
}

