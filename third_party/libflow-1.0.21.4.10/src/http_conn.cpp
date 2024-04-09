// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include "./utils.hpp"
#include "flow_inner.hpp"

namespace flow {

HttpConn::HttpConn(mg_connection* conn) : m_conn(conn) {}

int HttpConn::get_socket() const {
  return static_cast<int>(m_conn->sock);
}

void HttpConn::send_response(const HttpResponse& res) {
  dbglog("send response: %s", res.dump().c_str());
  int datalen = static_cast<int>(res.body.size());
  mg_send_head(m_conn, res.code, datalen, res.headers.c_str());
  mg_send(m_conn, res.body.data(), datalen);
}

void HttpConn::send_head(int code, int64_t content_length,
  const std::string& extra_headers) {
  dbglog("send head: %s\r\n%s", get_status_line(code).c_str(),
    extra_headers.c_str());
  mg_send_head(m_conn, code, content_length, extra_headers.c_str());
}

void HttpConn::send_body(const char* data, size_t size) {
  mg_send(m_conn, data, static_cast<int>(size));
}

void HttpConn::send_trunk(const char* data, size_t size) {
  mg_send(m_conn, data, static_cast<int>(size));
}

void HttpConn::close() {
  m_conn->flags |= MG_F_SEND_AND_CLOSE;
}

}  // namespace flow
