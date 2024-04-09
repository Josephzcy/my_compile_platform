// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <string>
#include <stdexcept>
#include "utils.hpp"
#include "flow_inner.hpp"

namespace flow {

Sender::Sender(Context* ctx, const char* name) {
  if (ctx == nullptr) {
    throw std::runtime_error("invalid flow context");
  }

  if (name == nullptr || strlen(name) == 0) {
    throw std::runtime_error("invalid sender name");
  }

  m_impl = new SenderImpl(ctx, name);
}

Sender::~Sender() {
  delete m_impl;
}

void Sender::send(const std::string& topic, const char* data, size_t size) {
  m_impl->send(topic, data, size);
}

}  // namespace flow
