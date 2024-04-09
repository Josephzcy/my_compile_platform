// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <sstream>
#include "flow_inner.hpp"

namespace flow {

std::string HttpRequest::get_url() const {
  if (query.size() == 0) {
    return uri;
  } else {
    return uri + "?" + query;
  }
}

std::string HttpRequest::dump() const {
  std::ostringstream oss;
  oss << method << " " << get_url() << " " << proto << "\r\n"
      << headers << "\r\n\r\n"
      << body;
  return oss.str();
}

}  // namespace flow
