// Copyright (C) 2019 - MiniEye INC.

#include <string.h>
#include <string>
#include <vector>
#include "String.h"

namespace string {
// Test if a str has the specified prefix.
bool has_prefix(const std::string& str, const std::string& prefix) {
  size_t prefix_len = prefix.size();
  return str.substr(0, prefix_len) == prefix;
}

// Test if a str has the specified suffix.
bool has_suffix(const std::string& str, const std::string& suffix) {
  size_t str_len = str.size();
  size_t suffix_len = suffix.size();
  if (str_len < suffix_len) {
    return false;
  }

  size_t start = str_len - suffix_len;
  return str.substr(start) == suffix;
}

// Find up to max occurances of a substr in str.
// If not found, return an empty vector.
// Otherwise, return the positions of occurances.
std::vector<int> find_many(const std::string& str,
  const std::string& substr, int max) {
  std::vector<int> indices;
  size_t str_len = str.size();
  size_t sub_len = substr.size();
  if (str_len == 0 || sub_len == 0 ||
      str_len < sub_len || max == 0) {
    return indices;
  }

  int found = 0;
  for (size_t i = 0; i < str_len; ) {
    int pos = static_cast<int>(str.find(substr, i));
    if (pos == std::string::npos) {
      break;
    } else {
      indices.push_back(pos);
      i = pos + sub_len;
      found += 1;
      if (max > 0 && found >= max) {
        break;
      }
    }
  }

  return indices;
}

// Split a string with a separator.
// maxsplit < 0: split at every occuranc
// maxsplit == 0: do not split
// maxsplit > 0: split maxsplit times (into maxsplit+1 parts)
std::vector<std::string> split(const std::string& str,
  const std::string& sep, int maxsplit) {
  std::vector<std::string> parts;
  auto indices = find_many(str, sep);
  int found = static_cast<int>(indices.size());
  if (found == 0 || maxsplit == 0) {
    parts.push_back(str);
    return parts;
  }

  if (maxsplit < 0) {
    maxsplit = found;
  } else if (maxsplit > found) {
    maxsplit = found;
  }

  int start = 0;
  size_t sub_len = sep.size();
  for (int i = 0; i < maxsplit; i += 1) {
    int index = indices.at(i);
    parts.push_back(str.substr(start, index - start));
    start = index + static_cast<int>(sub_len);
  }

  parts.push_back(str.substr(start));
  return parts;
}

// Split a string with a separator from right.
// maxsplit < 0: split at every occuranc
// maxsplit == 0: do not split
// maxsplit > 0: split maxsplit times (into maxsplit+1 parts)
std::vector<std::string> rsplit(const std::string& str,
  const std::string& sep, int maxsplit) {
  std::vector<std::string> parts;
  auto indices = find_many(str, sep);
  int found = static_cast<int>(indices.size());
  if (found == 0 || maxsplit == 0) {
    parts.push_back(str);
    return parts;
  }

  if (maxsplit < 0) {
    maxsplit = found;
  } else if (maxsplit > found) {
    maxsplit = found;
  }

  int first_split_index = found - maxsplit;

  int start = 0;
  size_t sub_len = sep.size();
  for (int i = first_split_index; i < found; i += 1) {
    int index = indices.at(i);
    parts.push_back(str.substr(start, index - start));
    start = index + static_cast<int>(sub_len);
  }

  parts.push_back(str.substr(start));
  return parts;
}

// Join the string array with glue.
// array.size() == 0, return ""
// array.size() == 1, return array[0]
// array.size() > 1, return the joined string
std::string join(const std::vector<std::string>& array,
  const std::string& glue) {
  size_t count = array.size();
  if (count == 0) {
    return "";
  } else if (count == 1) {
    return array[0];
  } else {
    std::string res = array[0];
    for (size_t i = 1; i < count; i += 1) {
      res += glue + array[i];
    }

    return res;
  }
}

// Remove heading chars from str.
std::string lstrip(const std::string& str,
  const std::string& chars) {
  if (chars.size() == 0) {
    return str;
  }

  auto pos = str.find_first_not_of(chars);
  if (pos == std::string::npos) {
    return "";
  } else {
    return str.substr(pos);
  }
}

// Remove trailing chars from str.
std::string rstrip(const std::string& str,
  const std::string& chars) {
  if (chars.size() == 0) {
    return str;
  }

  auto pos = str.find_last_not_of(chars);
  if (pos == std::string::npos) {
    return "";
  } else {
    return str.substr(0, pos + 1);
  }
}

// Remove both heading and trailing chars from str.
std::string strip(const std::string& str,
  const std::string& chars) {
  if (chars.size() == 0) {
    return str;
  }

  auto pos1 = str.find_first_not_of(chars);
  if (pos1 == std::string::npos) {
    return "";
  }

  auto pos2 = str.find_last_not_of(chars);
  return str.substr(pos1, pos2 - pos1 + 1);
}

// Substitute all substr in str with rep.
std::string gsub(const std::string& str,
  const std::string& substr, const std::string& rep) {
  size_t sub_len = substr.size();
  if (sub_len == 0) {
    return str;
  }

  auto indices = find_many(str, substr);
  size_t found = indices.size();
  if (found == 0) {
    return str;
  }

  std::string result = "";
  int start = 0;
  for (size_t i = 0; i < found; i += 1) {
    int index = indices[i];
    result += str.substr(start, index - start);
    result += rep;
    start = index + static_cast<int>(sub_len);
  }

  result += str.substr(start);
  return result;
}
}  // namespace string
