// Copyright (C) 2019 - MiniEye INC.

#pragma once

#include <string>
#include <vector>

namespace string {
// Test if a str has the specified prefix.
bool has_prefix(const std::string& str, const std::string& prefix);

// Test if a str has the specified suffix.
bool has_suffix(const std::string& str, const std::string& suffix);

// Find up to max occurances of a substr in str.
// If not found, return an empty vector.
// Otherwise, return the positions of occurances.
std::vector<int> find_many(const std::string& str,
  const std::string& substr, int max = -1);

// Split a string with a separator.
// maxsplit < 0: split at every occuranc
// maxsplit == 0: do not split
// maxsplit > 0: split maxsplit times (into maxsplit+1 parts)
std::vector<std::string> split(const std::string& str,
  const std::string& sep, int maxsplit = -1);

// Split a string with a separator from right.
// maxsplit < 0: split at every occuranc
// maxsplit == 0: do not split
// maxsplit > 0: split maxsplit times (into maxsplit+1 parts)
std::vector<std::string> rsplit(const std::string& str,
  const std::string& sep, int maxsplit = -1);

// Join the string array with glue.
// array.size() == 0, return ""
// array.size() == 1, return array[0]
// array.size() > 1, return the joined string
std::string join(const std::vector<std::string>& array,
  const std::string& glue);

// Remove heading chars from str.
std::string lstrip(const std::string& str,
  const std::string& chars = "\t\n\v\f\r ");

// Remove trailing chars from str.
std::string rstrip(const std::string& str,
  const std::string& chars = "\t\n\v\f\r ");

// Remove both heading and trailing chars from str.
std::string strip(const std::string& str,
  const std::string& chars = "\t\n\v\f\r ");

// Substitute all substr in str with rep.
std::string gsub(const std::string& str,
  const std::string& substr, const std::string& rep);
}  // namespace string
