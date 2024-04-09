/// @file Inspect.hpp
/// @brief Inspect the Value's content.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-08-30
/// Copyright (C) 2018 - MiniEye INC.

#include "Packer.hpp"

namespace Packer {

std::ostream& operator<<(std::ostream& os, const Value& v) {
  const char* tname = GetValueTypeName(v.Type);
  os << tname << "(";

  switch (v.Type) {
    case ValueType::Nil:
      break;
    case ValueType::Bool:
      os << v.Bool;
      break;
    case ValueType::Int8:
      os << v.Int8;
      break;
    case ValueType::Int16:
      os << v.Int16;
      break;
    case ValueType::Int32:
      os << v.Int32;
      break;
    case ValueType::Int64:
      os << v.Int64;
      break;
    case ValueType::UInt8:
      os << v.UInt8;
      break;
    case ValueType::UInt16:
      os << v.UInt16;
      break;
    case ValueType::UInt32:
      os << v.UInt32;
      break;
    case ValueType::UInt64:
      os << v.UInt64;
      break;
    case ValueType::Float32:
      os << v.Float32;
      break;
    case ValueType::Float64:
      os << v.Float64;
      break;
    case ValueType::CStr:
      os << v.CStr;
      break;
    case ValueType::PString:
      os << *(v.PString);
      break;
    case ValueType::PBlob:
      os.write(v.PBlob->Data, v.PBlob->Size);
      break;
    case ValueType::PPack:
      os.write(v.PPack->Data, v.PPack->Size);
      break;
    case ValueType::Array:
      for (auto i = 0u; i < v.Array->size(); i += 1) {
        if (i > 0) {
          os << ", ";
        }

        const Value& e = (*v.Array)[i];
        os << e;
      }

      break;
    case ValueType::Map:
      for (auto i = 0u; i < v.Map->size(); i += 1) {
        if (i > 0) {
          os << ", ";
        }

        const auto& kv = (*v.Map)[i];
        const auto& key = kv.first;
        const auto& val = kv.second;
        os << key << ": " << val;
      }

      break;
  }

  os << ")";
  return os;
}

}  // namespace Packer
