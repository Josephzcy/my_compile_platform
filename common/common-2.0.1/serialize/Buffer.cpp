/// @file Buffer.cpp
/// @brief Very simple stream buffer.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#include "./Buffer.hpp"

namespace Packer {

Buffer::Buffer() {
  mData.reserve(8192);
}

void Buffer::write(const char* data, size_t size) {
  mData.insert(mData.end(), data, data + size);
}

const char* Buffer::data() const {
  return mData.data();
}

size_t Buffer::size() const {
  return mData.size();
}

}  // namespace Packer
