/// @file log_level.h
/// @brief this file define some macros for vlog
/// @author Devin (devin@minieye.cc)

#ifndef  LOG_LEVEL_H
#define  LOG_LEVEL_H

#define CORE_LOG 0
#define SUBMODULE_LOG 10
#define DEBUG_LOG 20

#if defined(USE_GLOG)
#include "glog/logging.h"
#else
#include <iostream>
#include <assert.h>
#include "common/base/static_assert.h"
#define LOG(serverity) std::cout
#define VLOG(serverity) std::cout
#define LOG_IF(serverity, is_true) if(is_true) std::cout
#define CHECK(is_true) assert(is_true)
#endif

#endif  // LOG_LEVEL_H
