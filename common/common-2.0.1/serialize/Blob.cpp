/// @file Blob.cpp
/// @brief Binary data holder.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#include "Blob.hpp"

namespace Packer {

Blob::Blob() : Data(nullptr), Size(0) {
}

Blob::Blob(const char* ptr, size_t sz) : Data(ptr), Size(sz) {
}

Blob::Blob(const Blob& blob) : Data(blob.Data), Size(blob.Size) {
}

}  // namespace Packer
