/// @file Value.cpp
/// @brief The Value class.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-08-30
/// Copyright (C) 2018 - MiniEye INC.

#include "Packer.hpp"

namespace Packer {

Value::Value() : Type(ValueType::Nil) {
}

Value::Value(std::nullptr_t ptr) : Type(ValueType::Nil) {
}

Value::Value(bool v) : Type(ValueType::Bool) {
  Bool = v;
}

Value::Value(int8_t v) : Type(ValueType::Int8) {
  Int8 = v;
}

Value::Value(int16_t v) : Type(ValueType::Int16) {
  Int16 = v;
}

Value::Value(int32_t v) : Type(ValueType::Int32) {
  Int32 = v;
}

Value::Value(int64_t v) : Type(ValueType::Int64) {
  Int64 = v;
}

Value::Value(uint8_t v) : Type(ValueType::UInt8) {
  UInt8 = v;
}

Value::Value(uint16_t v) : Type(ValueType::UInt16) {
  UInt16 = v;
}

Value::Value(uint32_t v) : Type(ValueType::UInt32) {
  UInt32 = v;
}

Value::Value(uint64_t v) : Type(ValueType::UInt64) {
  UInt64 = v;
}

Value::Value(float v) : Type(ValueType::Float32) {
  Float32 = v;
}

Value::Value(double v) : Type(ValueType::Float64) {
  Float64 = v;
}

Value::Value(const char* v) : Type(ValueType::CStr) {
  CStr = v;
}

Value::Value(const std::string& v) : Type(ValueType::PString) {
  PString = &v;
}

Value::Value(const Blob& v) : Type(ValueType::PBlob) {
  PBlob = &v;
}

Value::Value(const Pack& v) : Type(ValueType::PPack) {
  PPack = &v;
}

Value::Value(const Value& v) : Type(v.Type) {
  copy_data(v);
}

Value::Value(std::initializer_list<Value> list) {
  bool is_map = true;
  for (const auto& v : list) {
    if (v.Type != ValueType::Array || v.Array->size() != 2) {
      is_map = false;
      break;
    }
  }

  if (is_map) {
    Type = ValueType::Map;
    Map = new std::vector<std::pair<Value, Value>>;
    Map->reserve(list.size());
    for (const auto& e : list) {
      const auto& key = (*e.Array)[0];
      const auto& val = (*e.Array)[1];
      Map->push_back({key, val});
    }
  } else {
    Type = ValueType::Array;
    Array = new std::vector<Value>(list);
  }
}

Value& Value::operator=(const Value& v) {
  destroy();
  Type = v.Type;
  copy_data(v);
  return *this;
}

Value::~Value() {
  destroy();
}

void Value::destroy() {
  if (Type == ValueType::Array && Array != nullptr) {
    delete Array;
    Array = nullptr;
  } else if (Type == ValueType::Map && Map != nullptr) {
    delete Map;
    Map = nullptr;
  }
}

void Value::copy_data(const Value& v) {
  switch (v.Type) {
    case ValueType::Nil:
      break;
    case ValueType::Bool:
      Bool = v.Bool;
      break;
    case ValueType::Int8:
      Int8 = v.Int8;
      break;
    case ValueType::Int16:
      Int16 = v.Int16;
      break;
    case ValueType::Int32:
      Int32 = v.Int32;
      break;
    case ValueType::Int64:
      Int64 = v.Int64;
      break;
    case ValueType::UInt8:
      UInt8 = v.UInt8;
      break;
    case ValueType::UInt16:
      UInt16 = v.UInt16;
      break;
    case ValueType::UInt32:
      UInt32 = v.UInt32;
      break;
    case ValueType::UInt64:
      UInt64 = v.UInt64;
      break;
    case ValueType::Float32:
      Float32 = v.Float32;
      break;
    case ValueType::Float64:
      Float64 = v.Float64;
      break;
    case ValueType::CStr:
      CStr = v.CStr;
      break;
    case ValueType::PString:
      PString = v.PString;
      break;
    case ValueType::PBlob:
      PBlob = v.PBlob;
      break;
    case ValueType::PPack:
      PPack = v.PPack;
      break;
    case ValueType::Array:
      Array = new std::vector<Value>(*v.Array);
      break;
    case ValueType::Map:
      Map = new std::vector<std::pair<Value, Value>>(*v.Map);
      break;
  }
}

}  // namespace Packer
