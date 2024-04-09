/// @file ToJSON.cpp
/// @brief Very simple JSON serializer.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-06
/// Copyright (C) 2018 - MiniEye INC.

#include "Packer.hpp"

namespace Packer {

Buffer Value::to_json() const {
  Buffer buf;
  write_json(buf);
  return buf;
}

}  // namespace Packer
