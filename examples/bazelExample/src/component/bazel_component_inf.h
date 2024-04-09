/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 * Author: Nicholas Zhang
 * Version: 1.0.0
 * Time: 2022-10-17 10:50:23
 *****************************************************************************/

#ifndef _BAZEL_COMPONENT_INF_H_
#define _BAZEL_COMPONENT_INF_H_

#ifdef WIN32
#define BAZEL_COMPONENT_INF_API  _declspec(dllexport)
#else
#define BAZEL_COMPONENT_INF_API    __attribute__ ((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

BAZEL_COMPONENT_INF_API int add(int a, int b);


#ifdef __cplusplus
}
#endif

#endif //_BAZEL_EX_INF_H_
