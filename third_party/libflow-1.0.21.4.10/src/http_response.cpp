// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <sstream>
#include "flow_inner.hpp"

namespace flow {

HttpResponse::HttpResponse() : HttpResponse(0) {}

HttpResponse::HttpResponse(int c) : HttpResponse(c, "", "") {}

HttpResponse::HttpResponse(int c, const std::string& h,
  const std::string& b) : code(c), headers(h), body(b) {}

HttpResponse::HttpResponse(int c, const StrPairVec& h,
  const std::string& b) : HttpResponse(c, encode_headers_vp(h), b) {}

std::string HttpResponse::dump() const {
  std::ostringstream oss;
  oss << get_status_line(code) << "\r\n"
      << headers << "\r\n\r\n"
      << body;
  return oss.str();
}

const std::map<int, std::string> HttpStatusMap = {
  {100, "Continue"},
  {101, "Switching Protocols"},
  {102, "Processing"},
  {200, "OK"},
  {100, "Continue"},
  {101, "Switching Protocols"},
  {200, "OK"},
  {201, "Created"},
  {202, "Accepted"},
  {203, "Non-Authoritative Information"},
  {204, "No Content"},
  {205, "Reset Content"},
  {206, "Partial Content"},
  {300, "Multiple Choices"},
  {301, "Moved Permanently"},
  {302, "Found"},
  {303, "See Other"},
  {304, "Not Modified"},
  {307, "Temporary Redirect"},
  {308, "Permanent Redirect"},
  {400, "Bad Request"},
  {401, "Unauthorized"},
  {403, "Forbidden"},
  {404, "Not Found"},
  {405, "Method Not Allowed"},
  {406, "Not Acceptable"},
  {407, "Proxy Authentication Required"},
  {408, "Request Timeout"},
  {409, "Conflict"},
  {410, "Gone"},
  {411, "Length Required"},
  {412, "Precondition Failed"},
  {413, "Payload Too Large"},
  {414, "URI Too Long"},
  {415, "Unsupported Media Type"},
  {416, "Range Not Satisfiable"},
  {417, "Expectation Failed"},
  {418, "I'm a teapot"},
  {426, "Upgrade Required"},
  {428, "Precondition Required"},
  {429, "Too Many Requests"},
  {431, "Request Header Fields Too Large"},
  {451, "Unavailable For Legal Reasons"},
  {500, "Internal Server Error"},
  {501, "Not Implemented"},
  {502, "Bad Gateway"},
  {503, "Service Unavailable"},
  {504, "Gateway Timeout"},
  {505, "HTTP Version Not Supported"},
  {511, "Network Authentication Required"},
};

std::string HttpResponse::get_status(int code) {
  auto it = HttpStatusMap.find(code);
  if (it != HttpStatusMap.end()) {
    return it->second;
  } else {
    return "Unknown Status";
  }
}

}  // namespace flow
