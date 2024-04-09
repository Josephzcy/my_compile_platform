/// @file ToMsgpack.cpp
/// @brief Very simple MessagePack serializer.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-08-30
/// Copyright (C) 2018 - MiniEye INC.

#include "Packer.hpp"

namespace Packer {

Buffer Value::to_msgpack() const {
  Buffer buf;
  write_msgpack(buf);
  return buf;
}

}  // namespace Packer
