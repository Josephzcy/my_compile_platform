/// @file Packer.hpp
/// @brief Very simple JSON/MessagePack serializer.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-08-30
/// Copyright (C) 2018 - MiniEye INC.

#ifndef PACKER_HPP_
#define PACKER_HPP_

#include <inttypes.h>
#include <initializer_list>
#include <iostream>
#include <cstring>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <functional>
#include "./Buffer.hpp"
#include "./Blob.hpp"
#include "./Pack.hpp"
#include "./ValueType.hpp"
#include "./JSONStream.hpp"
#include "./MsgpackStream.hpp"

namespace Packer {

class Value {
 public:
  Value();
  Value(std::nullptr_t ptr);
  Value(bool v);
  Value(int8_t v);
  Value(int16_t v);
  Value(int32_t v);
  Value(int64_t v);
  Value(uint8_t v);
  Value(uint16_t v);
  Value(uint32_t v);
  Value(uint64_t v);
  Value(float v);
  Value(double v);
  Value(const char* v);
  Value(const std::string& v);
  Value(const Blob& v);
  Value(const Pack& v);
  Value(const Value& v);

  template <typename T>
  Value(const std::vector<T>& list) : Type(ValueType::Array) {
    Array = new std::vector<Value>;
    Array->reserve(list.size());
    for (const auto& e : list) {
      Array->push_back(Value(e));
    }
  }

  template <typename K, typename V>
  Value(const std::map<K, V>& map) : Type(ValueType::Map) {
    Map = new std::vector<std::pair<Value, Value>>;
    Map->reserve(map.size());
    for (const auto& kv : map) {
      Map->push_back({kv.first, kv.second});
    }
  }

  Value(std::initializer_list<Value> list);
  Value& operator=(const Value& v);
  ~Value();

  // Show the Value's structure (for debugging).
  // e.g. std::cout << v << std::endl;
  friend std::ostream& operator<<(std::ostream& os, const Value& v);

  // Serialize to msgpack.
  Buffer to_msgpack() const;

  // Serialize to json.
  Buffer to_json() const;

  // Interface for msgpack::pack().
  // Usage:
  //   #include <msgpack.hpp>
  //   Value v { ... };
  //   msgpack::sbuffer sbuf;
  //   msgpack::pack(sbuf, v);
  template <typename Packer>
  void msgpack_pack(Packer& pk) const {
    switch (Type) {
      case ValueType::Nil:
        pk.pack_nil();
        break;
      case ValueType::Bool:
        pk.pack(Bool);
        break;
      case ValueType::Int8:
        pk.pack(Int8);
        break;
      case ValueType::Int16:
        pk.pack(Int16);
        break;
      case ValueType::Int32:
        pk.pack(Int32);
        break;
      case ValueType::Int64:
        pk.pack(Int64);
        break;
      case ValueType::UInt8:
        pk.pack(UInt8);
        break;
      case ValueType::UInt16:
        pk.pack(UInt16);
        break;
      case ValueType::UInt32:
        pk.pack(UInt32);
        break;
      case ValueType::UInt64:
        pk.pack(UInt64);
        break;
      case ValueType::Float32:
        pk.pack(Float32);
        break;
      case ValueType::Float64:
        pk.pack(Float64);
        break;
      case ValueType::CStr:
        {
          size_t len = std::strlen(CStr);
          pk.pack_str(len);
          pk.pack_str_body(CStr, len);
        }

        break;
      case ValueType::PString:
        {
          size_t len = PString->size();
          pk.pack_bin(len);
          pk.pack_bin_body(PString->data(), len);
        }

        break;
      case ValueType::PBlob:
        {
          size_t len = PBlob->Size;
          pk.pack_bin(len);
          pk.pack_bin_body(PBlob->Data, len);
        }

        break;
      case ValueType::PPack:
        pk.pack_bin_body(PPack->Data, PPack->Size);
        break;
      case ValueType::Array:
        pk.pack_array(Array->size());
        for (const auto& e : *Array) {
          pk.pack(e);
        }

        break;
      case ValueType::Map:
        pk.pack_map(Map->size());
        for (const auto& kv : *Map) {
          pk.pack(kv.first);
          pk.pack(kv.second);
        }

        break;
    }
  }

  template <typename Stream>
  void write_msgpack(Stream& os) const {
    msgpack::write_value(&os, *this);
  }

  template <typename Stream>
  void write_json(Stream& os) const {
    json::write_value(&os, *this);
  }

 public:
  ValueType Type;

  union {
    bool     Bool;
    int8_t   Int8;
    int16_t  Int16;
    int32_t  Int32;
    int64_t  Int64;
    uint8_t  UInt8;
    uint16_t UInt16;
    uint32_t UInt32;
    uint64_t UInt64;
    float    Float32;
    double   Float64;
    const char* CStr;
    const std::string* PString;
    const Blob* PBlob;
    const Pack* PPack;
    std::vector<Value>* Array;
    std::vector<std::pair<Value, Value>>* Map;
  };

 protected:
  void destroy();
  void copy_data(const Value& v);
};

}  // namespace Pack

#endif  // PACKER_HPP_
