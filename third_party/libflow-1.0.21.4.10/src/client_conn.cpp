// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <algorithm>
#include <string>
#include "./String.h"
#include "./platform.h"
#include "./utils.hpp"
#include "flow_inner.hpp"

namespace flow {

void ClientConn::Subscribe(const std::string& topic) {
  std::vector<std::string> parts = string::split(topic, ",");
  for (auto part : parts) {
    const std::string t = string::strip(part);
    if (t == "") {
      continue;
    }

    auto it = std::find(m_topics.begin(), m_topics.end(), t);
    if (it == m_topics.end()) {
      m_topics.push_back(t);
      dbglog("connection %p (%p) subscribed to %s",
        m_conn, this, t.c_str());
      // PrintVector(m_topics, "m_topics");
    }
  }
}

void ClientConn::Unsubscribe(const std::string& topic) {
  std::vector<std::string> parts = string::split(topic, ",");
  for (auto part : parts) {
    const std::string t = string::strip(part);
    if (t == "") {
      continue;
    }

    auto it = std::find(m_topics.begin(), m_topics.end(), t);
    if (it != m_topics.end()) {
      m_topics.erase(it);
      dbglog("connection %p (%p) unsubscribed from %s",
        m_conn, this, t.c_str());
      // PrintVector(m_topics, "m_topics");
    }
  }
}

bool ClientConn::MatchTopic(const std::string& topic) const {
  for (const std::string& s : m_topics) {
    bool match = wildcard_match(s.c_str(), topic.c_str());
    if (match) {
      return true;
    }
  }

  return false;
}

bool ClientConn::IsDrained() const {
  return m_conn->send_mbuf.len == 0;
}

bool ClientConn::IsMgConnDrained(const MgConn* conn) {
  return conn->send_mbuf.len == 0;
}

}  // namespace flow
