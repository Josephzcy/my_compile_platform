/// @file Pack.hpp
/// @brief Msgpack data chunk.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#ifndef PACK_HPP_
#define PACK_HPP_

#include <stddef.h>

namespace Packer {

// A Pack is a msgpack data chunk.
class Pack {
 public:
  Pack();
  Pack(const char* ptr, size_t sz);
  Pack(const Pack& pack);

 public:
  const char* Data;
  size_t Size;
};

}  // namespace Packer

#endif  // PACK_HPP_
