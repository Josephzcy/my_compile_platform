/// @file ValueType.hpp
/// @brief Supported element types.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#ifndef VALUETYPE_HPP_
#define VALUETYPE_HPP_

namespace Packer {

enum class ValueType {
  Nil,
  Bool,
  Int8,
  Int16,
  Int32,
  Int64,
  UInt8,
  UInt16,
  UInt32,
  UInt64,
  Float32,
  Float64,
  CStr,
  PString,
  PBlob,
  PPack,
  Array,
  Map,
};

const char* GetValueTypeName(const ValueType& t);

}  // namespace Packer

#endif  // VALUETYPE_HPP_
