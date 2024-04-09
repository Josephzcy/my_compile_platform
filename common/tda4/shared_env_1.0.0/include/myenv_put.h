// Copyright (C) 2019 - MiniEye INC.

#pragma once

#include <string>
namespace myenv {
int put(const std::string& key, const std::string& val);
int put_flags_file(const char* path);
int put_flags_text(const char* text);
}
