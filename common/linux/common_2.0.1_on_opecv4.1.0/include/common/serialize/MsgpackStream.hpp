/// @file MsgpackStream.hpp
/// @brief Write to a MsgPack stream.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#ifndef MSGPACKSTREAM_HPP_
#define MSGPACKSTREAM_HPP_

#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <utility>
#include "./ValueType.hpp"

// MsgPack mark ranges:
// [0x00, 0x7f]  // Positive Fixnum
// [0x80, 0x8f]  // FixMap
// [0x90, 0x9f]  // FixArray
// [0xa0, 0xbf]  // FixStr
// [0xc0, 0xdf]  // Variable
// [0xe0, 0xff]  // Negative Fixnum

#define MSGMARK_FIXMAP   0x80  // 10000000
#define MSGMARK_FIXARRAY 0x90  // 10010000
#define MSGMARK_FIXSTR   0xa0  // 10100000

#define MSGMARK_NIL      0xc0
#define MSGMARK_STRING   0xc1  // unused
#define MSGMARK_FALSE    0xc2
#define MSGMARK_TRUE     0xc3

#define MSGMARK_BIN8     0xc4
#define MSGMARK_BIN16    0xc5
#define MSGMARK_BIN32    0xc6

#define MSGMARK_EXT8     0xc7  // unused
#define MSGMARK_EXT16    0xc8  // unused
#define MSGMARK_EXT32    0xc9  // unused

#define MSGMARK_FLOAT    0xca
#define MSGMARK_DOUBLE   0xcb

#define MSGMARK_UINT8    0xcc
#define MSGMARK_UINT16   0xcd
#define MSGMARK_UINT32   0xce
#define MSGMARK_UINT64   0xcf

#define MSGMARK_INT8     0xd0
#define MSGMARK_INT16    0xd1
#define MSGMARK_INT32    0xd2
#define MSGMARK_INT64    0xd3

#define MSGMARK_FIXEXT1  0xd4  // unused
#define MSGMARK_FIXEXT2  0xd5  // unused
#define MSGMARK_FIXEXT4  0xd6  // unused
#define MSGMARK_FIXEXT8  0xd7  // unused
#define MSGMARK_FIXEXT16 0xd8  // unused

#define MSGMARK_STR8     0xd9
#define MSGMARK_STR16    0xda
#define MSGMARK_STR32    0xdb

#define MSGMARK_ARRAY16  0xdc
#define MSGMARK_ARRAY32  0xdd

#define MSGMARK_MAP16    0xde
#define MSGMARK_MAP32    0xdf

namespace Packer {

namespace msgpack {

template <typename PStream, typename T>
inline void write_data(PStream os, T data, size_t size) {
  os->write(reinterpret_cast<const char*>(data), size);
}

template <typename T>
void to_big_endian(T v, uint8_t buf[]) {
  size_t sz = sizeof v;
  auto ptr = reinterpret_cast<const uint8_t*>(&v);
  for (size_t i = 0; i < sz; i += 1) {
    buf[sz - i - 1] = ptr[i];
  }
}

template <typename PStream, typename T>
void write_num(PStream os, uint8_t mark, T val) {
  uint8_t buf[sizeof(T) + 1] = {mark};
  to_big_endian(val, buf + 1);
  write_data(os, buf, sizeof buf);
}

template <typename PStream>
void write_null(PStream os) {
  char mark = MSGMARK_NIL;
  write_data(os, &mark, 1);
}

template <typename PStream>
void write_bool(PStream os, bool v) {
  char mark = v ? MSGMARK_TRUE : MSGMARK_FALSE;
  write_data(os, &mark, 1);
}

template <typename PStream>
void write_uint8(PStream os, uint8_t v) {
  if (v <= INT8_MAX) {  // fixnum
    write_data(os, &v, sizeof v);
  } else {  // unsigned 8
    write_num(os, MSGMARK_UINT8, v);
  }
}

template <typename PStream>
void write_uint16(PStream os, uint16_t v) {
  if (v <= UINT8_MAX) {  // unsigned 8
    write_uint8(os, static_cast<uint8_t>(v));
  } else {  // unsigned 16
    write_num(os, MSGMARK_UINT16, v);
  }
}

template <typename PStream>
void write_uint32(PStream os, uint32_t v) {
  if (v <= UINT16_MAX) {  // unsigned 16
    write_uint16(os, static_cast<uint16_t>(v));
  } else {  // unsigned 32
    write_num(os, MSGMARK_UINT32, v);
  }
}

template <typename PStream>
void write_uint64(PStream os, uint64_t v) {
  if (v <= UINT32_MAX) {  // unsigned 32
    write_uint32(os, static_cast<uint32_t>(v));
  } else {  // unsigned 64
    write_num(os, MSGMARK_UINT64, v);
  }
}

template <typename PStream>
void write_int8(PStream os, int8_t v) {
  if (v < -(1 << 5)) {  // signed 8
    write_num(os, MSGMARK_INT8, v);
  } else {  // fixnum
    write_data(os, &v, sizeof v);
  }
}

template <typename PStream>
void write_int16(PStream os, int16_t v) {
  if (v < INT8_MIN) {  // signed 16
    write_num(os, MSGMARK_INT16, v);
  } else if (v <= INT8_MAX) {  // fixnum
    write_int8(os, static_cast<int8_t>(v));
  } else if (v <= UINT8_MAX) {  // unsigned 8
    write_uint8(os, static_cast<uint8_t>(v));
  } else {  // unsigned 16
    write_uint16(os, static_cast<uint16_t>(v));
  }
}

template <typename PStream>
void write_int32(PStream os, int32_t v) {
  if (v < INT16_MIN) {  // signed 32
    write_num(os, MSGMARK_INT32, v);
  } else if (v <= INT16_MAX) {  // signed 16
    write_int16(os, static_cast<int16_t>(v));
  } else if (v <= UINT16_MAX) {  // unsigned 16
    write_uint16(os, static_cast<uint16_t>(v));
  } else {  // unsigned 32
    write_uint32(os, static_cast<uint32_t>(v));
  }
}

template <typename PStream>
void write_int64(PStream os, int64_t v) {
  if (v < INT32_MIN) {  // signed 64
    write_num(os, MSGMARK_INT64, v);
  } else if (v <= INT32_MAX) {  // signed 32
    write_int32(os, static_cast<int32_t>(v));
  } else if (v <= UINT32_MAX) {  // unsigned 32
    write_uint32(os, static_cast<uint32_t>(v));
  } else {  // unsigned 64
    write_uint64(os, static_cast<uint64_t>(v));
  }
}

template <typename PStream>
void write_float32(PStream os, float v) {
  write_num(os, MSGMARK_FLOAT, v);
}

template <typename PStream>
void write_float64(PStream os, double v) {
  write_num(os, MSGMARK_DOUBLE, v);
}

// str must be NULL-terminated.
template <typename PStream>
void write_str(PStream os, const char* str, size_t size) {
  if (size <= 31) {
    uint8_t mark = (MSGMARK_FIXSTR | static_cast<uint8_t>(size));
    write_data(os, &mark, 1);
  } else if (size <= UINT8_MAX) {
    write_num(os, MSGMARK_STR8, static_cast<uint8_t>(size));
  } else if (size <= UINT16_MAX) {
    write_num(os, MSGMARK_STR16, static_cast<uint16_t>(size));
  } else if (size <= UINT32_MAX) {
    write_num(os, MSGMARK_STR32, static_cast<uint32_t>(size));
  }

  os->write(str, size);
}

template <typename PStream>
void write_bin(PStream os, const char* data, size_t size) {
  if (size <= UINT8_MAX) {
    uint8_t buf[2] = {MSGMARK_BIN8, static_cast<uint8_t>(size)};
    write_data(os, buf, sizeof buf);
  } else if (size <= UINT16_MAX) {
    write_num(os, MSGMARK_BIN16, static_cast<uint16_t>(size));
  } else if (size <= UINT32_MAX) {
    write_num(os, MSGMARK_BIN32, static_cast<uint32_t>(size));
  }

  os->write(data, size);
}

template <typename PStream, typename T>
void write_value(PStream os, const T& v);

template <typename PStream, typename T, typename E>
void write_array(PStream os, const T& array) {
  size_t size = array->size();
  if (size <= 15) {
    uint8_t mark = (MSGMARK_FIXARRAY | static_cast<uint8_t>(size));
    write_data(os, &mark, 1);
  } else if (size <= UINT16_MAX) {
    write_num(os, MSGMARK_ARRAY16, static_cast<uint16_t>(size));
  } else if (size <= UINT32_MAX) {
    write_num(os, MSGMARK_ARRAY32, static_cast<uint32_t>(size));
  }

  for (size_t i = 0; i < size; i += 1) {
    const E& e = array->at(i);
    write_value(os, e);
  }
}

template <typename PStream, typename T, typename K, typename V>
void write_map(PStream os, const T& map) {
  size_t size = map->size();
  if (size <= 15) {
    uint8_t mark = (MSGMARK_FIXMAP | static_cast<uint8_t>(size));
    write_data(os, &mark, 1);
  } else if (size <= UINT16_MAX) {
    write_num(os, MSGMARK_MAP16, static_cast<uint16_t>(size));
  } else if (size <= UINT32_MAX) {
    write_num(os, MSGMARK_MAP32, static_cast<uint32_t>(size));
  }

  for (size_t i = 0; i < size; i += 1) {
    const std::pair<K, V>& p = map->at(i);
    write_value(os, p.first);
    write_value(os, p.second);
  }
}

template <typename PStream, typename T>
void write_value(PStream os, const T& v) {
  switch (v.Type) {
    case ValueType::Nil:
      write_null(os);
      break;
    case ValueType::Bool:
      write_bool(os, v.Bool);
      break;
    case ValueType::Int8:
      write_int8(os, v.Int8);
      break;
    case ValueType::Int16:
      write_int16(os, v.Int16);
      break;
    case ValueType::Int32:
      write_int32(os, v.Int32);
      break;
    case ValueType::Int64:
      write_int64(os, v.Int64);
      break;
    case ValueType::UInt8:
      write_uint8(os, v.UInt8);
      break;
    case ValueType::UInt16:
      write_uint16(os, v.UInt16);
      break;
    case ValueType::UInt32:
      write_uint32(os, v.UInt32);
      break;
    case ValueType::UInt64:
      write_uint64(os, v.UInt64);
      break;
    case ValueType::Float32:
      write_float32(os, v.Float32);
      break;
    case ValueType::Float64:
      write_float64(os, v.Float64);
      break;
    case ValueType::CStr:
      write_str(os, v.CStr, strlen(v.CStr));
      break;
    case ValueType::PString:
      write_str(os, v.PString->data(), v.PString->size());
      break;
    case ValueType::PBlob:
      write_bin(os, v.PBlob->Data, v.PBlob->Size);
      break;
    case ValueType::PPack:
      os->write(v.PPack->Data, v.PPack->Size);
      break;
    case ValueType::Array:
      write_array<PStream, decltype(v.Array), T>(os, v.Array);
      break;
    case ValueType::Map:
      write_map<PStream, decltype(v.Map), T, T>(os, v.Map);
      break;
  }
}

}  // namespace msgpack

}  // namespace Packer

#endif  // MSGPACKSTREAM_HPP_

// Refer to: msgpack-c/include/msgpack/v1/pack.hpp
