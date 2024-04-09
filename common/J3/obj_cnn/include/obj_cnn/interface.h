// Copyright 2023 MINIEYE

#ifndef INCLUDE_OBJ_CNN_INTERFACE_H_
#define INCLUDE_OBJ_CNN_INTERFACE_H_

#include <string>

#include "obj_cnn/code.h"

namespace obj_cnn {

template <typename T>
char const *ToStr(const T &);

template <typename T>
std::string ToString(const T &t) {
  return ToStr<T>(t);
}

template <typename T>
Code TryFromStr(const char *text, T *) {
  return CODE_TODO;
}

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_INTERFACE_H_
