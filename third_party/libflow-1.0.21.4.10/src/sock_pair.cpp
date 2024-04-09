// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <mongoose.h>
#include "./platform.h"
#include "./utils.hpp"
#include "flow_inner.hpp"

namespace flow {

SockPair::SockPair() {
  int rc = mg_socketpair(m_socks, SOCK_DGRAM);
  if (rc == 0) {
    throw std::runtime_error("mg_socketpair failed with 0");
  }
}

// `sock_type` can be either `SOCK_STREAM` or `SOCK_DGRAM`.
SockPair::SockPair(int sock_type) {
  int rc = mg_socketpair(m_socks, sock_type);
  if (rc == 0) {
    throw std::runtime_error("mg_socketpair failed with 0");
  }
}

SockPair::~SockPair() {
  close_socket(&(m_socks[0]));
  close_socket(&(m_socks[1]));
}

sock_t& SockPair::End0() {
  return m_socks[0];
}

sock_t& SockPair::End1() {
  return m_socks[1];
}

}  // namespace flow
