// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#ifndef _UTILS_H_
#define _UTILS_H_

#if _pragma_once_support
#pragma once
#endif

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace flow {

enum LogLevel {
  Verbose,
  Debug,
  Info,
  Warning,
  Error,
  Fatal,
};

extern LogLevel g_log_level;

template <typename T>
void PrintVector(const std::vector<T>& vec, const std::string& name) {
  int i = 0;
  std::cerr << "vector " << name << " size: " << vec.size() << std::endl;
  for (const T& entry : vec) {
    std::cerr << "vec[" << i++ << "]: " << entry << std::endl;
  }
}

template <typename T>
void PrintVec(const std::vector<T>& vec) {
  int i = 0;
  std::cout << "[";

  for (auto v : vec) {
    if (i > 0) {
      std::cout << ", ";
    }

    std::cout << v;
    ++i;
  }

  std::cout << "]" << std::endl;
}

template <typename KeyType, typename ValType>
void PrintMap(const std::map<KeyType, ValType>& m) {
  int i = 0;
  std::cout << "{";

  for (auto kv : m) {
    if (i > 0) {
      std::cout << ", ";
    }

    std::cout << kv.first << ": " << kv.second;
    ++i;
  }

  std::cout << "}" << std::endl;
}

std::string int_to_string(int num);
std::string int64_to_string(int64_t num);

int string_to_int(const std::string& str);
int64_t string_to_int64(const std::string& str);

std::string join_strings(const std::vector<std::string>& vec,
  const std::string& sep);
std::vector<std::string> split_string(const std::string& str,
  const std::string& sep);

std::vector<std::pair<std::string, std::string>>
parse_key_value_list(const std::string& str,
  const std::string& entry_sep, const std::string& kv_sep);

std::string merge_key_value_list(
  const std::vector<std::pair<std::string, std::string>>& vp,
  const std::string& entry_sep, const std::string& kv_sep);

template <typename T>
void vector_append_nonexist(std::vector<T>* vec, const T& value) {
  auto beg = vec->begin();
  auto end = vec->end();
  auto it = std::find(beg, end, value);
  if (it == end) {
    vec->push_back(value);
  }
}

template <typename T>
void vector_remove_value(std::vector<T>* vec, const T& value) {
  auto beg = vec->begin();
  auto end = vec->end();
  auto it = std::find(beg, end, value);
  if (it != end) {
    vec->erase(it);
  }
}

template <typename T1, typename T2>
std::map<T1, T2> tuple_list_to_map(const std::vector<std::pair<T1, T2>>& vp) {
  std::map<T1, T2> m;

  for (auto p : vp) {
    m[p.first] = p.second;
  }

  return m;
}

template <typename T1, typename T2>
std::vector<std::pair<T1, T2>> map_to_tuple_list(const std::map<T1, T2>& m) {
  std::vector<std::pair<T1, T2>> vp;

  for (auto kv : m) {
    vp.push_back({kv.first, kv.second});
  }

  return vp;
}

void sleep_ms(int ms);

int write_file(const char* file_path, const char* data, size_t size);

}  // namespace flow

#define flow_log(level, ...) {                         \
  if (flow::g_log_level <= level) {                    \
    char str[4096];                                    \
    snprintf(str, sizeof str, __VA_ARGS__);            \
    if (level >= flow::Error) {                        \
      fprintf(stderr, "[flow:" #level "] %s:%d: %s\n", \
        __FILE__, __LINE__, str);                      \
    } else {                                           \
      fprintf(stderr, "[flow:" #level "] %s\n", str);  \
    }                                                  \
  }                                                    \
}

#define verbolog(...) {                 \
  flow_log(flow::Verbose, __VA_ARGS__); \
}

#define dbglog(...) {                   \
  flow_log(flow::Debug, __VA_ARGS__);   \
}

#define infolog(...) {                  \
  flow_log(flow::Info, __VA_ARGS__);    \
}

#define warnlog(...) {                  \
  flow_log(flow::Warning, __VA_ARGS__); \
}

#define errlog(...) {                   \
  flow_log(flow::Error, __VA_ARGS__);   \
}

#define fatallog(...) {                 \
  flow_log(flow::Fatal, __VA_ARGS__);   \
}

#define panic(...) {              \
  flow_log(Fatal, __VA_ARGS__);   \
  exit(1);                        \
}

#endif
