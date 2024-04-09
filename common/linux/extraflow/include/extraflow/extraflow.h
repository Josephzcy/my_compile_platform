// Copyright (c) 2011 The Extraflow Authors. All rights reserved.

#ifndef INCLUDE_EXTRAFLOW_EXTRAFLOW_H_
#define INCLUDE_EXTRAFLOW_EXTRAFLOW_H_

#include <chrono>
#include <string>
// #include <iostream>

#include <flow.hpp>  // libflow
#include <msgpack.hpp>

namespace extraflow {

// Wrapper of flow::Context, make sure we use raw protocol.
class Context {
 public:
  using Inner = flow::Context;

  Inner const &inner() const { return context_; }
  Inner &inner() { return context_; }

  Context();
  Context(flow::Config config);

 private:
  flow::Context context_;
};

// Wrapper for flow::Sender to send data with extra infomation.
class Sender {
 public:
  using Inner = flow::Sender;

  Inner const &inner() const { return sender_; }
  Inner &inner() { return sender_; }

  Sender(Context *context, char const *name)
      : sender_(&(context->inner()), name), name_(name) {}

  // With extra data.
  template <typename ExtraT>
  void Send(std::string const &topic, char const *data, size_t size,
            ExtraT const &extra) {
    auto buf = PackStandardData(topic, data, size, true);
    msgpack::pack(*buf, extra);

    return sender_.send(topic, buf->data(), buf->size());
  }

  // No extra data.
  void Send(std::string const &topic, char const *data, size_t size);

 private:
  // Pack topic, source, time and data as a named map into a thread local
  // string buffer, if extra is true, is pack string "extra" and left value of
  // name "extra" unpacked.
  //
  // This function return pointer to the string buffer.
  msgpack::sbuffer *PackStandardData(std::string const &topic, char const *data,
                                     size_t size, bool extra);

  flow::Sender sender_;
  std::string name_;
};

}  // namespace extraflow

#endif  // INCLUDE_EXTRAFLOW_EXTRAFLOW_H_
