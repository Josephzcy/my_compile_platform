/// @file Blob.hpp
/// @brief Binary data holder.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#ifndef BLOB_HPP_
#define BLOB_HPP_

#include <stddef.h>

namespace Packer {

// A Blob is a binary data chunk.
class Blob {
 public:
  Blob();
  Blob(const char* ptr, size_t sz);
  Blob(const Blob& blob);

 public:
  const char* Data;
  size_t Size;
};

}  // namespace Packer

#endif  // BLOB_HPP_
