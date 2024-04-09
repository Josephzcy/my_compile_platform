/// @file utils.cpp
/// @brief Some useful functions.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2017-12-01
/// Copyright (C) 2017 - MiniEye INC.

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "common/fs/utils.h"

int isfile(const char* path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

bool isfile(const std::string& path) {
  return isfile(path.c_str()) != 0;
}

int isdir(const char* path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

bool isdir(const std::string& path) {
  return isdir(path.c_str()) != 0;
}
