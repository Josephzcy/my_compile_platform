// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include "./platform.h"
#include "utils.hpp"
#include "flow_inner.hpp"

namespace flow {

SenderImpl::SenderImpl(Context* ctx, const char* name) :
  m_context_impl(reinterpret_cast<ContextImpl*>(ctx->m_impl)),
  m_name(name) {
}

SenderImpl::SenderImpl(ContextImpl* ctx_impl, const char* name) :
  m_context_impl(ctx_impl), m_name(name) {
}

void SenderImpl::send(
  const std::string& topic,  // '\0' terminated string
  const char* data,          // any binary data
  size_t size) {             // <= 2^32
  if (m_context_impl->m_started) {
    auto format = m_context_impl->m_config["sender.serialize"];
    OutMsg* msg = nullptr;

    if (format == "raw") {
      msg = new OutMsgRaw();
      msg->Init(m_name, topic, data, size, 0);
    } else {
      msg = new OutMsg();
      msg->Init(m_name, topic, data, size, get_time_in_usecs_i64());
    }

    m_context_impl->m_server->PushOutMsg(msg);
  } else {
    dbglog("the server has not been started");
  }
}

}  // namespace flow
