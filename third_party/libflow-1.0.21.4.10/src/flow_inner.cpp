// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <mongoose.h>
#include <map>
#include <deque>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <msgpack.hpp>
#include "./platform.h"
#include "./utils.hpp"
#include "flow_inner.hpp"

namespace flow {

//----------------------------------------------------------
// class Receiver
//----------------------------------------------------------

Receiver::Receiver() {
}

Receiver::~Receiver() {
}

StrPairVec decode_query_vp(const std::string& str) {
  return parse_key_value_list(str, "&", "=");
}

StrMap decode_query_map(const std::string& str) {
  return tuple_list_to_map(decode_query_vp(str));
}

StrPairVec decode_headers_vp(const std::string& str) {
  return parse_key_value_list(str, "\r\n", ": ");
}

StrMap decode_headers_map(const std::string& str) {
  return tuple_list_to_map(decode_headers_vp(str));
}

std::string encode_query_vp(const StrPairVec& o) {
  return merge_key_value_list(o, "&", "=");
}

std::string encode_query_map(const StrMap& o) {
  return encode_query_vp(map_to_tuple_list(o));
}

std::string encode_headers_vp(const StrPairVec& o) {
  return merge_key_value_list(o, "\r\n", ": ");
}

std::string encode_headers_map(const StrMap& o) {
  return encode_headers_vp(map_to_tuple_list(o));
}

std::string get_status_line(int code) {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << code << " " << HttpResponse::get_status(code);
  return oss.str();
}

//----------------------------------------------------------
// class HttpHandler
//----------------------------------------------------------

HttpHandler::HttpHandler() {
}

HttpHandler::~HttpHandler() {
}

//----------------------------------------------------------
// class Http404Handler
//----------------------------------------------------------

int Http404Handler::handle(HttpConn* conn, const HttpRequest& req) {
  HttpResponse res {
    404,
    {
      {"Content-Type", "text/plain"},
    },
    "Not Found",
  };

  conn->send_response(res);
  conn->close();
  return 0;
}

//----------------------------------------------------------
// class RealClientInfo
//----------------------------------------------------------

void RealClientInfo::HandleClientEvent(MgConn* c, int ev, void* p) {
  switch (ev) {
    case MG_EV_CONNECT: {
      int status = *reinterpret_cast<int*>(p);  // 0 is Success
      if (status != 0) {
        errlog("client websocket connection error: %s", strerror(status));
        // Examples:
        // 111 Connection refused
      }

      break;
    }
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      dbglog("client websocket handshake done");
      m_state = ClientConnState::CONNECTED;
      DoSubscribe();
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *)p;
      verbolog("client websocket received a message frame (%zu bytes)",
        wm->size);
      int opcode = wm->flags & 0xf;
      HandleWebsocketMessage(opcode,
        reinterpret_cast<char*>(wm->data), wm->size);
      break;
    }
    case MG_EV_CLOSE: {
      dbglog("client websocket closed");
      m_state = ClientConnState::CLOSED;
      m_close_time = get_time_in_msecs_i64();
      break;
    }
  }
}

void RealClientInfo::HandleWebsocketMessage(int opcode,
                                            char* data,
                                            size_t size) {
  bool is_binary = (opcode == WEBSOCKET_OP_BINARY);
  if (!is_binary) {
    errlog("invalid websocket opcode: %d", opcode);
    return;
  }

  if (m_client) {
    if (m_ctx_impl->m_config["sender.serialize"] == "raw") {
      m_client->recv("", "", data, size);
    } else {
      std::shared_ptr<msgpack::object_handle> oh(new msgpack::object_handle);

      *oh = msgpack::unpack(data, size);  // copy?
      msgpack::object deserialized = oh->get();

      std::map<std::string, msgpack::object> dst;
      deserialized.convert(dst);  // copy?

      const std::string& source = dst["source"].convert();
      const std::string& topic = dst["topic"].convert();
      const std::string& data1 = dst["data"].convert();

      m_client->recv(source.c_str(), topic.c_str(), data1.c_str(), data1.size());
    }
  }
}

void RealClientInfo::DoSubscribe() {
  std::map<std::string, std::string> msg;
  msg["source"] = m_config.name;
  msg["topic"] = "subscribe";
  msg["data"] = m_config.topics;

  msgpack::sbuffer sb;
  msgpack::pack(sb, msg);

  mg_send_websocket_frame(m_conn, WEBSOCKET_OP_BINARY, sb.data(), sb.size());
}

bool RealClientInfo::IsWritable() const {
  return m_conn->send_mbuf.len < CLIENT_SEND_MBUF_LEN_MAX;
}

//----------------------------------------------------------
// class ContextImpl
//----------------------------------------------------------

ContextImpl::ContextImpl() : ContextImpl({
    {"addr", "127.0.0.1"},
    {"port", "24012"},
  }) {
}

ContextImpl::ContextImpl(const Config& config) :
  m_started(false), m_config(config),
  m_server(new WebServer(this, m_config)) {
  init_log_level();
  infolog("version %s", version);
}

ContextImpl::~ContextImpl() {
}

// 0: success, otherwise: error
int ContextImpl::start() {
#ifdef _WIN32
  if (init_windows() != 0) {
    return 1;
  }
#endif

  int rc = m_server->Start();
  if (rc != 0) {
    errlog("cannot start the web server");
    return 1;
  }

  printf("%lf\n", get_time_in_secs_d());
  m_started = true;
  return 0;
}

void ContextImpl::stop() {
  SenderImpl sender(this, "ContextImpl");
  sender.send("flow.context.eventloop.stop", "", 0);
  m_started = false;
}

void ContextImpl::add_receiver(Receiver* receiver) {
  m_server->add_receiver(receiver);
}

void ContextImpl::remove_receiver(Receiver* receiver) {
  m_server->remove_receiver(receiver);
}

void ContextImpl::add_http_handler(HttpHandler* handler) {
  m_server->add_http_handler(handler);
}

void ContextImpl::remove_http_handler(HttpHandler* handler) {
  m_server->remove_http_handler(handler);
}

void ContextImpl::add_client(Client* client) {
  m_server->add_client(client);
}

void ContextImpl::remove_client(Client* client) {
  m_server->remove_client(client);
}

void ContextImpl::init_log_level() {
  const char* level = getenv("LIBFLOW_LOG_LEVEL");
  if (level == nullptr) {
    g_log_level = Warning;
  } else if (strcmp(level, "Verbose") == 0) {
    g_log_level = Verbose;
  } else if (strcmp(level, "Debug") == 0) {
    g_log_level = Debug;
  } else if (strcmp(level, "Info") == 0) {
    g_log_level = Info;
  } else if (strcmp(level, "Warning") == 0) {
    g_log_level = Warning;
  } else if (strcmp(level, "Error") == 0) {
    g_log_level = Error;
  } else if (strcmp(level, "Fatal") == 0) {
    g_log_level = Fatal;
  }
}

Context ctx0;

const char* version = "0.21.4.10";

}  // namespace flow
