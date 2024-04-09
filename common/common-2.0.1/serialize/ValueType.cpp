/// @file ValueType.cpp
/// @brief Awesome program.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#include "./ValueType.hpp"

namespace Packer {

const char* GetValueTypeName(const ValueType& t) {
  switch (t) {
    case ValueType::Nil:
      return "Nil";
    case ValueType::Bool:
      return "Bool";
    case ValueType::Int8:
      return "Int8";
    case ValueType::Int16:
      return "Int16";
    case ValueType::Int32:
      return "Int32";
    case ValueType::Int64:
      return "Int64";
    case ValueType::UInt8:
      return "UInt8";
    case ValueType::UInt16:
      return "UInt16";
    case ValueType::UInt32:
      return "UInt32";
    case ValueType::UInt64:
      return "UInt64";
    case ValueType::Float32:
      return "Float32";
    case ValueType::Float64:
      return "Float64";
    case ValueType::CStr:
      return "CStr";
    case ValueType::PString:
      return "PString";
    case ValueType::PBlob:
      return "PBlob";
    case ValueType::PPack:
      return "PPack";
    case ValueType::Array:
      return "Array";
    case ValueType::Map:
      return "Map";
    default:
      return nullptr;
  }
}

}  // namespace Packer
