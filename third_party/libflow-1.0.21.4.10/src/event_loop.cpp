// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <mongoose.h>
#include <algorithm>
#include <string>
#include "./platform.h"
#include "./utils.hpp"
#include "flow_inner.hpp"

namespace flow {

static void client_handler(MgConn *c, int ev, void *p) {
  auto real_client = reinterpret_cast<RealClientInfo*>(c->user_data);
  real_client->HandleClientEvent(c, ev, p);
}

EventLoop::EventLoop(WebServer* server) {
  m_running = false;
  m_server = server;
  mg_mgr_init(&m_mg_mgr, nullptr);  // no return value
}

EventLoop::~EventLoop() {
  mg_mgr_free(&m_mg_mgr);  // no return value
}

void EventLoop::Run() {
  m_running = true;
  while (m_running) {
    m_server->DoClientConnection();

    mg_mgr_poll(&m_mg_mgr, 1000);
  }
}

void EventLoop::Stop() {
  m_running = false;
}

int EventLoop::AddHttpServer(const std::string& listen_addr,
  MgEventHandler handler, void* user_data) {
  struct mg_bind_opts opts;
  opts.user_data = user_data;
  opts.flags = 0;
  opts.error_string = nullptr;
  opts.iface = nullptr;

  MgConn *nc = mg_bind_opt(&m_mg_mgr,
    listen_addr.c_str(), handler, opts);
  if (nc == nullptr) {
    errlog("failed to listen on [%s]", listen_addr.c_str());
    return 1;
  }

  mg_set_protocol_http_websocket(nc);

  ServerInfo info({listen_addr, handler, user_data, nc});
  m_servers.push_back(info);
  return 0;
}

int EventLoop::AddPlainSocket(const sock_t& sock,
  MgEventHandler handler, void* user_data) {
  struct mg_add_sock_opts opts;
  opts.user_data = user_data;
  opts.flags = 0;
  opts.error_string = nullptr;
  opts.iface = nullptr;

  MgConn* nc = mg_add_sock_opt(&m_mg_mgr, sock, handler, opts);
  if (nc == nullptr) {
    errlog("failed to add socket %d", static_cast<int>(sock));
    return 1;
  }

  return 0;
}

void EventLoop::AddClient(RealClientInfo* real_client) {
  struct mg_connect_opts opts;
  opts.user_data = real_client;
  opts.flags = 0;
  opts.error_string = nullptr;
  opts.iface = nullptr;
  opts.nameserver = nullptr;

  const char* url = real_client->m_config.url.c_str();
  MgConn* nc = mg_connect_ws_opt(&m_mg_mgr,
    client_handler, opts, url, nullptr, nullptr);
  if (nc == nullptr) {
    errlog("failed to connect %s", url);
    return;
  }

  real_client->m_conn = nc;
  real_client->m_state = ClientConnState::CONNECTING;
}

}  // namespace flow
