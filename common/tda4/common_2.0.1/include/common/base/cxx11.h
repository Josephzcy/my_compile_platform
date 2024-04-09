#ifndef COMMON_BASE_CXX11_H
#define COMMON_BASE_CXX11_H
#pragma once

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || \
    defined __MSC_VER && __MSC_VER >= 1600
#define CXX11_ENABLED 1
#endif

#endif // COMMON_BASE_CXX11_H
