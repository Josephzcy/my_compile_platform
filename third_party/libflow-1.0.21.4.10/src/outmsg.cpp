// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <string>
#include <vector>
#include <msgpack.hpp>
#include "./flow_inner.hpp"

namespace flow {

#define PACK_STR(pk, str, len)              \
  {                                         \
    auto _len = static_cast<uint32_t>(len); \
    pk.pack_str(_len);                      \
    pk.pack_str_body(str, _len);            \
  }

#define PACK_BIN(pk, data, len)             \
  {                                         \
    auto _len = static_cast<uint32_t>(len); \
    pk.pack_bin(_len);                      \
    pk.pack_bin_body(data, _len);           \
  }

#define PACK_OUT_MSG(pk, source, topic, data, len, time) \
  {                                                      \
    pk.pack_map(4);                                      \
                                                         \
    PACK_STR(pk, "time", 4);                             \
    pk.pack_int64(time);                                 \
                                                         \
    PACK_STR(pk, "source", 6);                           \
    PACK_STR(pk, source.data(), source.size());          \
                                                         \
    PACK_STR(pk, "topic", 5);                            \
    PACK_STR(pk, topic.data(), topic.size());            \
                                                         \
    PACK_STR(pk, "data", 4);                             \
    PACK_BIN(pk, data, size);                            \
  }

OutMsg::OutMsg() : m_opaque(nullptr) {}

OutMsg::~OutMsg() {
  if (m_opaque) {
    auto mpbuf = reinterpret_cast<msgpack::sbuffer*>(m_opaque);
    delete mpbuf;
    m_opaque = nullptr;
  }
}

void OutMsg::Init(const std::string& source,  // '\0' terminated string
                  const std::string& topic,   // '\0' terminated string
                  const char*        data,    // any binary data
                  size_t             size,    // <= 2^32
                  int64_t            time) {  // micro-seconds from epoch
  m_topic = topic;
  auto mpbuf = new msgpack::sbuffer;  // throws std::bad_alloc
  msgpack::packer<msgpack::sbuffer> pk(mpbuf);
  PACK_OUT_MSG(pk, source, topic, data, size, time);
  m_opaque = reinterpret_cast<void*>(mpbuf);
}

const std::string& OutMsg::GetTopic() const {
  return m_topic;
}

BinRef OutMsg::GetSerializedData() const {
  auto mpbuf = reinterpret_cast<msgpack::sbuffer*>(m_opaque);
  return BinRef{mpbuf->data(), mpbuf->size()};
}

void OutMsgRaw::Init(const std::string& source,
                     const std::string& topic,
                     const char*        data,
                     size_t             size,
                     int64_t            time) {
  m_topic = topic;
  m_data.assign(data, data + size);
}

BinRef OutMsgRaw::GetSerializedData() const {
  return BinRef{m_data.data(), m_data.size()};
}

}  // namespace flow
