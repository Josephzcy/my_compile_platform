// Copyright 2019 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2019-08-20

#include <string>
#include "./platform.h"
#include "./utils.hpp"
#include "flow_inner.hpp"

namespace flow {

ClientOutMsg::ClientOutMsg() :
  m_client(nullptr), m_outmsg(nullptr) {}

ClientOutMsg::ClientOutMsg(Client* client, OutMsg* outmsg) :
  m_client(client), m_outmsg(outmsg) {}

ClientOutMsg::~ClientOutMsg() {
  if (m_outmsg) {
    delete m_outmsg;
  }
}

Client::Client(const ClientConfig& config) {
  auto real_client = new RealClientInfo;
  real_client->m_config = config;
  real_client->m_client = this;
  real_client->m_ctx_impl = nullptr;
  real_client->m_conn = nullptr;
  real_client->m_close_time = 0;
  real_client->m_state = ClientConnState::CLOSED;
  m_opaque = real_client;
}

Client::~Client() {
  auto real_client = reinterpret_cast<RealClientInfo*>(m_opaque);
  delete real_client;
}

void Client::send(const std::string& topic,  // '\0' terminated string
                  const char* data,          // any binary data
                  size_t size) {             // < 2^32
  auto real_client = reinterpret_cast<RealClientInfo*>(m_opaque);
  if (!real_client->m_ctx_impl) {
    errlog("the client has not been added");
    return;
  }

  int64_t now = get_time_in_usecs_i64();
  auto msg = new OutMsg();
  msg->Init("client", topic, data, size, now);
  auto cmsg = new ClientOutMsg(this, msg);

  std::unique_ptr<WebServer>& server = real_client->m_ctx_impl->m_server;
  server->PushClientOutMsg(cmsg);
}

// This function will be run in an event loop.
// DO NOT do blocking I/O or heavy computation in this function.
// The data will be dropped after calling this function. If you
// want to use it in future time, make your own copy.
void Client::recv(const char* source,  // '\0' terminated string
                  const char* topic,   // any binary data
                  const char* data,    // any binary data
                  size_t size) {       // < 2^32
  // do nothing
}

}  // namespace flow
