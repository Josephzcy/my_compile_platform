// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <string>
#include <sstream>
#include <chrono>  // NOLINT(build/c++11)
#include <thread>  // NOLINT(build/c++11)
#include <utility>
#include "utils.hpp"

namespace flow {

LogLevel g_log_level = Info;

std::string int_to_string(int num) {
  char buf[32];
  memset(buf, 0, sizeof buf);
  snprintf(buf, sizeof buf, "%d", num);
  return buf;
}

std::string int64_to_string(int64_t num) {
  char buf[32];
  memset(buf, 0, sizeof buf);
  snprintf(buf, sizeof buf, "%" PRIi64, num);
  return buf;
}

int string_to_int(const std::string& str) {
  return atoi(str.c_str());
}

int64_t string_to_int64(const std::string& str) {
  return atoll(str.c_str());
}

std::string join_strings(const std::vector<std::string>& vec,
  const std::string& sep) {
  std::ostringstream oss;

  for (size_t i = 0; i < vec.size(); ++i) {
    if (i > 0) {
      oss << sep;
    }

    oss << vec.at(i);
  }

  return oss.str();
}

std::vector<std::string>
split_string(const std::string& str, const std::string& sep) {
  std::vector<std::string> res;
  if (str.size() == 0) {
    return res;
  }

  if (sep.size() == 0) {
    for (size_t i = 0; i < str.size(); ++i) {
      res.push_back(str.substr(i, 1));
    }

    return res;
  }

  size_t pos = 0;
  while (true) {
    auto pos1 = str.find(sep, pos);
    if (pos1 == std::string::npos) {
      res.push_back(str.substr(pos));
      break;
    } else {
      res.push_back(str.substr(pos, pos1 - pos));
      pos = pos1 + sep.size();
    }
  }

  return res;
}

std::vector<std::pair<std::string, std::string>>
parse_key_value_list(const std::string& str,
  const std::string& entry_sep, const std::string& kv_sep) {
  std::vector<std::pair<std::string, std::string>> vp;

  auto entries = split_string(str, entry_sep);
  for (auto entry : entries) {
    if (entry.size() == 0) {  // empty string
      continue;
    }

    auto frags = split_string(entry, kv_sep);
    if (frags.size() == 2) {
      vp.push_back({frags[0], frags[1]});
    }
  }

  return vp;
}

std::string merge_key_value_list(
  const std::vector<std::pair<std::string, std::string>>& vp,
  const std::string& entry_sep, const std::string& kv_sep) {
  std::vector<std::string> v;
  for (auto kv : vp) {
    v.push_back(kv.first + kv_sep + kv.second);
  }

  return join_strings(v, entry_sep);
}

void sleep_ms(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int write_file(const char* file_path, const char* data, size_t size) {
  FILE* fp = fopen(file_path, "wb");
  if (fp == nullptr) {
    errlog("cannot open %s for write: %s", file_path, strerror(errno));
    return 1;
  }

  if (data != nullptr && size > 0) {
    size_t cnt = fwrite(data, 1, size, fp);
    if (cnt < size) {
      errlog("error writing %s: imcomplete write", file_path);

      if (fclose(fp) != 0) {
        errlog("error closing %s: %s", file_path, strerror(errno));
      }

      return 2;
    }
  }

  if (fclose(fp) != 0) {
    errlog("error closing %s: %s", file_path, strerror(errno));
    return 3;
  }

  return 0;
}

}  // namespace flow
