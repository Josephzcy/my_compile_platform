#ifndef R_MINIEYE_SOFTWARE_GROUP_DDS_COMMON_H
#define R_MINIEYE_SOFTWARE_GROUP_DDS_COMMON_H

#include <inttypes.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
DDS: Data Distribution Service
IPC: Interprocess Communication
*/

// IPC_TYPE_POSIX: use Posix IPC, used in linux.
const uint32_t IPC_TYPE_POSIX = 1;

// IPC_TYPE_SYSTEMV: use System V IPC, used in linux.
const uint32_t IPC_TYPE_SYSTEMV = 2;

// IPC_TYPE_SYSTEMV: use System V IPC huge table, used in linux.
const uint32_t IPC_TYPE_SYSTEMV_HUGETLB = 3;

// IPC_TYPE_ASHMEM: android ashmem, used in android system.
const uint32_t IPC_TYPE_ASHMEM = 4;

// IPC_TYPE_ANON: mmap shared, only shared in parent and child.
const uint32_t IPC_TYPE_ANON = 5;

// IPC_TYPE_HEAP: mmap private.
const uint32_t IPC_TYPE_HEAP = 6;

// IPC_TYPE_MALLOC: malloc.
const uint32_t IPC_TYPE_MALLOC = 7;

/*
shm log level
*/
const uint32_t RSHM_LOG_LEVEL_ERROR = 1;
const uint32_t RSHM_LOG_LEVEL_WARN = 2;
const uint32_t RSHM_LOG_LEVEL_INFO = 3;
const uint32_t RSHM_LOG_LEVEL_DEBUG = 4;
const uint32_t RSHM_LOG_LEVEL_TRACE = 5;

/*
@brief: init log, there is no need to call this function,
        if you don't want dds log.
@param loglevel[in]: 1: Error, 2: Warn, 3: Info, 4: Debug, 5: Trace
*/
void rshm_init(uint32_t loglevel);

/*
@brief: Unlink posix shared memory. Unlink will remove shared memory
        from operating system.
@param key[in]: key is the name for shared memory.
@return On success: 0.
        On error: error code is returned.
*/
int rshm_unlink_posix(const char *key);

/*
@brief: Like rshm_unlink_posix, but it will unlink multi shared memory.
@param key[in]: key is the name for shared memory.
@param num[in]: unlink "key[1-num]" shared memory.
@return On success: 0.
        On error: error code is returned.
@example:
  when name = "test", num = 3, it will unlink
    "test1", "test2", "test3"
*/
int rshm_unlink_multi_posix(const char *key, size_t num);

/*
@brief: Unlink SystemV shared memory. Unlink will remove shared memory
        from operating system.
@param key[in]: key is the name for shared memory.
        key should be in format (0x12345), hex number.
@return On success: 0.
        On error: error code is returned.
*/
int rshm_unlink_svpic(const char *key);

/*
@brief: Like rshm_unlink_svpic, but it will unlink multi shared memory.
@param key[in]: key is the name for shared memory.
@param num[in]: unlink "key[1-num]" shared memory.
@return On success: 0.
        On error: error code is returned.
@example:
  when name = "0x1234", num = 3, it will unlink
    "0x12341", "0x12342", "0x12343"
*/
int rshm_unlink_multi_svpic(const char *key, size_t num);

/*
Error Code.
[-2147483648, -1]: Custom error code.
0: Success
[1, 2147483647]: System Error Code
*/
const int32_t DDSSuccess = 0;
const int32_t ErrUnexpectedError = -1;
const int32_t ErrCannotAllocateMemory = -10;
const int32_t ErrMemoryLimitExceeded = -11;
const int32_t ErrBadPointerOrAddress = -12;
const int32_t ErrTimeOut = -13;
const int32_t ErrInvalidParam = -20;
const int32_t ErrInvalidID = -21;
const int32_t ErrInvalidIDTooSmall = -22;
const int32_t ErrInvalidIDTooBig = -23;
const int32_t ErrNotExist = -30;
const int32_t ErrAlreadyExist = -31;
const int32_t ErrBiggerSize = -32;
const int32_t ErrNoData = -40;
const int32_t ErrInvalidTopic = -50;
const int32_t ErrBadMagic = -51;
const int32_t ErrBadVersion = -52;
const int32_t ErrBadHeadSize = -53;
const int32_t ErrBadShmSize = -54;
const int32_t ErrBadNodeSize = -55;
const int32_t ErrBadNodeNum = -56;
const int32_t ErrBadFreeNodeNum = -57;
const int32_t ErrAlreadyInitialized = -58;
const int32_t ErrNotInitialized = -59;
const int32_t ErrUniqIDChanged = -80;
const int32_t ErrNoReadCallBack = -81;
const int32_t ErrNixInvalidPath = -90;
const int32_t ErrNixInvalidUtf8 = -91;
const int32_t ErrNixUnsupportedOperation = -92;
const int32_t ErrIO = -100;
const int32_t ErrAddrParse = -110;
const int32_t ErrNul = -120;
const int32_t ErrSerdeJson = -130;
const int32_t ErrAnyHow = -140;

#ifdef __cplusplus
}
#endif

#endif
