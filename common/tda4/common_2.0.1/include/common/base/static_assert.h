#ifndef COMMON_BASE_STATIC_ASSERT_H
#define COMMON_BASE_STATIC_ASSERT_H

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || \
    defined __MSC_VER && __MSC_VER >= 1600 || defined(_WIN32)
#define STATIC_ASSERT(e, ...) static_assert(e, "" __VA_ARGS__)
#else

// #include <type_traits>
#include "common/base/preprocess.h"

template <bool x> struct static_assertion_failure;

template <> struct static_assertion_failure<true> {
    enum { value = 1 };
};

template<int x> struct static_assert_test {};

/// 编译期间的静态断言
/// @param e 常量表达式
/// 用于编译期间检查错误的场合，示例：
/// @code
/// STATIC_ASSERT(sizeof(Foo) == 48, "Foo 的大小必须为 48");
/// @endcode
#define static_assert(e, ...) \
typedef static_assert_test< \
            sizeof(static_assertion_failure<(bool)(e)>)> \
            PP_JOIN(static_assert_failed, __LINE__)

#define STATIC_ASSERT(e, ...) \
typedef static_assert_test< \
            sizeof(static_assertion_failure<(bool)(e)>)> \
            PP_JOIN(static_assert_failed, __LINE__)
#endif


#endif // COMMON_BASE_STATIC_ASSERT_H

