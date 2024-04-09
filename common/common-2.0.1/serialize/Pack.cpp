/// @file Pack.cpp
/// @brief Msgpack data chunk.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#include "./Pack.hpp"

namespace Packer {

Pack::Pack() : Data(nullptr), Size(0) {
}

Pack::Pack(const char* ptr, size_t sz) : Data(ptr), Size(sz) {
}

Pack::Pack(const Pack& pack) : Data(pack.Data), Size(pack.Size) {
}

}  // namespace Packer
