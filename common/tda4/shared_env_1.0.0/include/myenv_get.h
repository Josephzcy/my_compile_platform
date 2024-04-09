// Copyright (C) 2019 - MiniEye INC.

#pragma once

#include <string>
#include <stdint.h>
#include <vector>
namespace myenv {
std::string get(const std::string& name, const std::string& defval = "");

// non-exist: false
// exists, but has empty value: true
// exists, value is 0/0.0/false/no: false
// exists, value is others: true
bool get_bool(const std::string& name, bool deval = false);

// non-exist: defval
// exists, strtol(str)
int32_t get_int32(const std::string& name, int32_t defval = 0);

// non-exist: defval
// exists, strtoll(str)
int64_t get_int64(const std::string& name, int64_t defval = 0);

// non-exist: defval
// exists, atof(str)
float get_float32(const std::string& name, float defval = 0.0);

// non-exist: defval
// exists, atof(str)
double get_float64(const std::string& name, double defval = 0.0);
}  // namespace myenv
