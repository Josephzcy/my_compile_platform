/// @file Buffer.hpp
/// @brief Very simple stream buffer.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include <stddef.h>
#include <vector>

namespace Packer {

// Holds the serialized data of Value.
class Buffer {
 public:
  Buffer();
  void write(const char* data, size_t size);
  const char* data() const;
  size_t size() const;

 protected:
  std::vector<char> mData;
};

}  // namespace Packer

#endif  // BUFFER_HPP_
