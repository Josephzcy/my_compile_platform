// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include "flow_inner.hpp"

namespace flow {

Context::Context() {
  m_impl = new ContextImpl;
}

Context::Context(const Config& config) {
  m_impl = new ContextImpl(config);
}

Context::~Context() {
  delete m_impl;
}

const Context& Context::operator=(Context&& ctx) {
  delete m_impl;
  m_impl = ctx.m_impl;
  ctx.m_impl = nullptr;
  return *this;
}

int Context::start() {
  return m_impl->start();
}

void Context::stop() {
  m_impl->stop();
}

void Context::add_receiver(Receiver* receiver) {
  m_impl->add_receiver(receiver);
}

void Context::remove_receiver(Receiver* receiver) {
  m_impl->remove_receiver(receiver);
}

void Context::add_http_handler(HttpHandler* handler) {
  m_impl->add_http_handler(handler);
}

void Context::remove_http_handler(HttpHandler* handler) {
  m_impl->remove_http_handler(handler);
}

void Context::add_client(Client* client) {
  m_impl->add_client(client);
}

void Context::remove_client(Client* client) {
  m_impl->remove_client(client);
}

}  // namespace flow
