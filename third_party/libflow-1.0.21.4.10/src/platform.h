/// @file platform.h
/// @brief Platform-dependent code.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2020-11-04
/// Copyright (C) 2020 - MiniEye INC.

#pragma once

#include <stdint.h>
#include <mongoose.h>

bool wildcard_match(const char *pattern, const char *str);

int thread_setname(const char* name);

void close_socket(sock_t* sock);

// Current UNIX time
double get_time_in_secs_d();
int64_t get_time_in_msecs_i64();
int64_t get_time_in_usecs_i64();

// Return value: 0=OK, others=Error
int init_windows();

int clean_windows();
