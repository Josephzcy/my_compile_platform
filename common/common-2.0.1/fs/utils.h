/// @file utils.h
/// @brief Some useful functions.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2017-12-01
/// Copyright (C) 2017 - MiniEye INC.

#ifndef COMMON_FS_UTILS_H_
#define COMMON_FS_UTILS_H_

#include <string>

int isfile(const char* path);
bool isfile(const std::string& path);
int isdir(const char* path);
bool isdir(const std::string& path);

#endif  // COMMON_FS_UTILS_H_
