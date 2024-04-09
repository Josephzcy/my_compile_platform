/// @file JSONStream.cpp
/// @brief Write to a JSON stream.
/// @author zhangqi (zhangqi@minieye.cc)
/// @date 2018-09-07
/// Copyright (C) 2018 - MiniEye INC.

#ifndef JSONSTREAM_HPP_
#define JSONSTREAM_HPP_

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <utility>
#include "./ValueType.hpp"

namespace Packer {

namespace json {

template <typename PStream>
void write_null(PStream os) {
  os->write("null", 4);
}

template <typename PStream>
void write_bool(PStream os, bool v) {
  if (v) {
    os->write("true", 4);
  } else {
    os->write("false", 5);
  }
}

template <typename PStream, typename T>
void write_fmt(PStream os, const char* format, const T& v) {
  char buf[64];
  snprintf(buf, sizeof buf, format, v);
  os->write(buf, strlen(buf));
}

// str must be NULL-terminated.
template <typename PStream>
void write_str(PStream os, const char* str, size_t size = UINT32_MAX) {
  os->write("\"", 1);

  for (size_t i = 0; i < size; i += 1) {
    if (str[i] == '\0') {
      break;
    } else if (str[i] == '"') {
      os->write("\\\"", 2);
    } else {
      os->write(str + i, 1);
    }
  }

  os->write("\"", 1);
}

template <typename PStream>
void write_bin(PStream os, const char* data, size_t size) {
  os->write("\"", 1);

  const char tab[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz0123456789+/";

  size_t osize = ((size + 2) / 3) * 4;
  if (osize > size) {
    char buf[4];

    const char* beg = data;
    const char* end = data + size;

    while (end - beg >= 3) {
      buf[0] = tab[beg[0] >> 2];
      buf[1] = tab[((beg[0] & 0x3) << 4) | (beg[1] >> 4)];
      buf[2] = tab[((beg[1] & 0xf) << 2) | (beg[2] >> 6)];
      buf[3] = tab[beg[3] & 0x3f];
      os->write(buf, 4);
      beg += 3;
    }

    if (end - beg) {
      buf[3] = '=';
      buf[0] = tab[beg[0] >> 2];

      if (end - beg == 1) {
        buf[1] = tab[(beg[1] & 0x3) << 4];
        buf[2] = '=';
      } else {
        buf[1] = tab[((beg[0] & 0x3) << 4) | (beg[1] >> 4)];
        buf[2] = tab[((beg[1] & 0xf) << 2)];
      }

      os->write(buf, 4);
    }
  }

  os->write("\"", 1);
}


template <typename PStream, typename T1, typename T2>
void write_pair(PStream os, const std::pair<T1, T2>& p) {
  p.first.write_json(*os);
  os->write(":", 1);
  p.second.write_json(*os);
}

template <typename PStream, typename T, typename F>
void write_list(PStream os, const T& list, F write_entry) {
  for (size_t i = 0; i < list->size(); i += 1) {
    if (i > 0) {
      os->write(",", 1);
    }

    const auto& e = list->at(i);
    write_entry(os, e);
  }
}

template <typename PStream, typename T>
void write_value(PStream os, const T& v);

template <typename PStream, typename T, typename E>
void write_array(PStream os, const T& array) {
  os->write("[", 1);
  write_list(os, array, write_value<PStream, const E&>);
  os->write("]", 1);
}

template <typename PStream, typename T, typename K, typename V>
void write_map(PStream os, const T& map) {
  os->write("{", 1);
  write_list(os, map, write_pair<PStream, K, V>);
  os->write("}", 1);
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
      write_fmt(os, "%d", v.Int8);
      break;
    case ValueType::Int16:
      write_fmt(os, "%d", v.Int16);
      break;
    case ValueType::Int32:
      write_fmt(os, "%d", v.Int32);
      break;
    case ValueType::Int64:
      write_fmt(os, "%" PRId64, v.Int64);
      break;
    case ValueType::UInt8:
      write_fmt(os, "%d", v.UInt8);
      break;
    case ValueType::UInt16:
      write_fmt(os, "%d", v.UInt16);
      break;
    case ValueType::UInt32:
      write_fmt(os, "%d", v.UInt32);
      break;
    case ValueType::UInt64:
      write_fmt(os, "%" PRIu64, v.UInt64);
      break;
    case ValueType::Float32:
      write_fmt(os, "%f", v.Float32);
      break;
    case ValueType::Float64:
      write_fmt(os, "%lf", v.Float64);
      break;
    case ValueType::CStr:
      write_str(os, v.CStr);
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

}  // namespace json

}  // namespace Packer

#endif  // JSONSTREAM_HPP_
