// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <msgpack.hpp>
#include <atomic>  // NOLINT(build/include_order)
#include "./platform.h"  // NOLINT(build/include_subdir)
#include "./flow_inner.hpp"
#include "./utils.hpp"

namespace flow {

std::string mg_str_to_string(const struct mg_str& o) {
  return std::string(o.p, o.len);
}

StrPairVec get_headers_from_mg_http_message(const struct http_message& o) {
  StrPairVec vp;

  for (int i = 0; i < MG_MAX_HTTP_HEADERS; ++i) {
    auto key = mg_str_to_string(o.header_names[i]);
    if (key.size() == 0) {
      break;
    }

    auto val = mg_str_to_string(o.header_values[i]);
    vp.push_back({key, val});
  }

  return vp;
}

HttpRequest convert_mg_http_message(const struct http_message& hm) {
  auto vp = get_headers_from_mg_http_message(hm);
  auto headers_str = merge_key_value_list(vp, "\r\n", ": ");

  return {
    mg_str_to_string(hm.proto),
    mg_str_to_string(hm.method),
    mg_str_to_string(hm.uri),
    mg_str_to_string(hm.query_string),
    headers_str,
    mg_str_to_string(hm.body),
  };
}

//----------------------------------------------------------
// Mongoose Event Handlers
//----------------------------------------------------------

static void http_handler(MgConn *c, int ev, void *p) {
  WebServer* server = reinterpret_cast<WebServer*>(c->user_data);
  server->HandleHttpEvent(c, ev, p);
}

static void sender_sock_handler(MgConn *c, int ev, void *p) {
  WebServer* server = reinterpret_cast<WebServer*>(c->user_data);
  server->HandleSenderSockEvent(c, ev, p);
}

static void client_sock_handler(MgConn *c, int ev, void *p) {
  WebServer* server = reinterpret_cast<WebServer*>(c->user_data);
  server->HandleClientSockEvent(c, ev, p);
}

//----------------------------------------------------------
// class WebServer
//----------------------------------------------------------

WebServer::WebServer(ContextImpl* ctx_impl) :
  m_ctx_impl(ctx_impl),
  m_config({
    {"addr", "127.0.0.1"},
    {"port", "24012"},
  }),
  m_sender_socks(nullptr),
  m_client_socks(nullptr),
  m_loop(nullptr),
  m_conn_counter(0),
  m_clients_conn_time(0) {
}

WebServer::WebServer(ContextImpl* ctx_impl, const Config& config) :
  m_ctx_impl(ctx_impl),
  m_config(config),
  m_sender_socks(nullptr),
  m_client_socks(nullptr),
  m_loop(nullptr),
  m_conn_counter(0),
  m_clients_conn_time(0) {
}

WebServer::~WebServer() {
}

void WebServer::add_receiver(Receiver* receiver) {
  if (receiver == nullptr) {
    return;
  }

  std::lock_guard<std::mutex> guard(m_receivers_lock);
  vector_append_nonexist(&m_receivers, receiver);
}

void WebServer::remove_receiver(Receiver* receiver) {
  if (receiver == nullptr) {
    return;
  }

  std::lock_guard<std::mutex> guard(m_receivers_lock);
  vector_remove_value(&m_receivers, receiver);
}

void WebServer::add_http_handler(HttpHandler* handler) {
  if (handler == nullptr) {
    return;
  }

  std::lock_guard<std::mutex> guard(m_http_handlers_lock);
  vector_append_nonexist(&m_http_handlers, handler);
}

void WebServer::remove_http_handler(HttpHandler* handler) {
  if (handler == nullptr) {
    return;
  }

  std::lock_guard<std::mutex> guard(m_http_handlers_lock);
  vector_remove_value(&m_http_handlers, handler);
}

void WebServer::add_client(Client* client) {
  if (client == nullptr || client->m_opaque == nullptr) {
    return;
  }

  auto real_client = reinterpret_cast<RealClientInfo*>(client->m_opaque);
  real_client->m_ctx_impl = m_ctx_impl;

  std::lock_guard<std::mutex> guard(m_clients_lock);

  // check existence
  auto it = std::find(m_real_clients.begin(), m_real_clients.end(),
    real_client);
  if (it != m_real_clients.end()) {  // already added
    return;
  }

  m_real_clients.emplace_back(real_client);
}

void WebServer::remove_client(Client* client) {
  if (client == nullptr || client->m_opaque == nullptr) {
    return;
  }

  auto real_client = reinterpret_cast<RealClientInfo*>(client->m_opaque);

  std::lock_guard<std::mutex> guard(m_clients_lock);

  auto it = std::find(m_real_clients.begin(), m_real_clients.end(),
    real_client);
  if (it == m_real_clients.end()) {  // not added
    return;
  }

  vector_remove_value(&m_real_clients, real_client);
  real_client->m_conn->flags |= MG_F_CLOSE_IMMEDIATELY;  // close the connection
}

int WebServer::GetQueueCapacityConfig(const std::string& topic) {
  auto key = "queue." + topic + ".capacity";
  auto it = m_config.find(key);
  if (it == m_config.end()) {
    return 8;
  }

  auto str = it->second;
  return string_to_int(str);
}

int WebServer::SetQueueCapacityConfig(const std::string& topic, int capacity) {
  auto it = m_queues.find(topic);
  if (it != m_queues.end()) {
    errlog("cannot set queue capacity: the queue has been created");
    return 1;
  }

  auto key = "queue." + topic + ".capacity";
  m_config[key] = int_to_string(capacity);
  return 0;
}

static std::atomic<int8_t> start_code(-1);

int WebServer::Start() {
  m_thread = std::thread(&WebServer::Run, this);
  m_thread.detach();

  for (int i = 0; i < 100; i += 1) {
    sleep_ms(50);
    if (start_code != -1) {
      break;
    }
  }

  int code = start_code.load();
  if (code == 0) {  // ok
    return 0;
  } else if (code == -1) {  // unchanged, timeout
    errlog("Error: WebServer start notifier timeout");
    return 2;
  } else {  // error
    errlog("Error: WebServer failed to start (code %d)", code);
    return 3;
  }
}

void WebServer::Run() {
  thread_setname("FlowServer");

  if (m_config["pusher.socket.type"] == "udp") {
    m_sender_socks = new SockPair(SOCK_DGRAM);
    m_client_socks = new SockPair(SOCK_DGRAM);
  } else {
    m_sender_socks = new SockPair(SOCK_STREAM);
    m_client_socks = new SockPair(SOCK_STREAM);
  }

  if (m_config["sender.drain_mode"] == "most") {
    m_drain_mode = DRAIN_MODE_MOST;
  } else {
    m_drain_mode = DRAIN_MODE_ANY;
  }

  m_loop = new EventLoop(this);

  auto servers = m_config["servers"];
  if (servers != "none") {
    auto bind_addr = m_config["addr"] + ":" + m_config["port"];
    int rc = m_loop->AddHttpServer(bind_addr, http_handler, this);
    if (rc != 0) {
      start_code = 1;
      return;
    }

    rc = m_loop->AddPlainSocket(m_sender_socks->End1(),
      sender_sock_handler, this);
    if (rc != 0) {
      start_code = 2;
      return;
    }
  }

  int rc = m_loop->AddPlainSocket(m_client_socks->End1(),
    client_sock_handler, this);
  if (rc != 0) {
    start_code = 3;
    return;
  }

  start_code = 0;  // successfully started

  m_loop->Run();

  delete m_loop;
  delete m_client_socks;
  delete m_sender_socks;

  dbglog("the server has finished");
  return;
}

void WebServer::HandleHttpEvent(MgConn* c, int ev, void* p) {
  if (ev == MG_EV_ACCEPT) {
    dbglog("accepted connection %p", c);
  } else if (ev == MG_EV_HTTP_REQUEST) {
    OnRecvHTTPRequest(c, p);
  } else if (ev == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
    OnRecvWSEstablished(c);
  } else if (ev == MG_EV_WEBSOCKET_FRAME) {
    OnRecvWSFrame(c, p);
  } else if (ev == MG_EV_SEND) {
    OnRecvClientDrain(c, p);
  } else if (ev == MG_EV_CLOSE) {
    OnRecvClientClose(c);
  }
}

void WebServer::HandleSenderSockEvent(MgConn* c, int ev, void *p) {
  if (ev == MG_EV_RECV) {
    HandleSenderSockRecvEvent(c);
  }
}

void WebServer::HandleClientSockEvent(MgConn* c, int ev, void *p) {
  if (ev == MG_EV_RECV) {
    HandleClientSockRecvEvent(c);
  }
}

void WebServer::HandleSenderSockRecvEvent(MgConn* c) {
  const int ptr_size = sizeof(const OutMsg*);
  struct mbuf& buf = c->recv_mbuf;
  while (buf.len >= ptr_size) {
    auto msg = *reinterpret_cast<const OutMsg**>(buf.buf);
    if (msg) {
      const std::string topic = msg->GetTopic();
      if (topic.find("flow.context.") == 0) {
        OnRecvContextMessage(topic, msg);
      } else {
        OnRecvOutputMessage(topic, msg);
      }
    }

    mbuf_remove(&buf, ptr_size);
  }
}

void WebServer::HandleClientSockRecvEvent(MgConn* c) {
  const int ptr_size = sizeof(const ClientOutMsg*);
  struct mbuf& buf = c->recv_mbuf;
  while (buf.len >= ptr_size) {
    auto msg = *reinterpret_cast<const ClientOutMsg**>(buf.buf);
    if (msg) {
      OnRecvClientOutputMessage(msg);
      delete msg;
    }

    mbuf_remove(&buf, ptr_size);
  }
}

void WebServer::OnRecvContextMessage(const std::string& topic,
  const OutMsg* msg) {
  if (topic == "flow.context.eventloop.stop") {
    m_loop->Stop();
  }
}

void WebServer::OnRecvOutputMessage(const std::string& topic,
  const OutMsg* msg) {
  // create a queue for the topic, if not exists
  auto it = m_queues.find(topic);
  if (it == m_queues.end()) {
    int capacity = GetQueueCapacityConfig(topic);
    dbglog("set queue capacity to %d for topic %s",
      capacity, topic.c_str());
    m_queues[topic] = OutMsgQueue(capacity, 0);
  }

  // get the queue
  auto& queue = m_queues[topic];

  dbglog("OnRecvOutputMessage: "
         "received an outmsg with topic: %s, queue length: %zu",
         topic.c_str(), queue.size());

  // drop the oldest message
  if (queue.full()) {
    queue.pop();
    dbglog("OnRecvOutputMessage: "
           "dropped an outmsg with topic %s", topic.c_str());
  }

  // enqueue the new message
  queue.push(OutMsgSP(msg));
  CheckTopic(topic);
}

void WebServer::OnRecvClientOutputMessage(const ClientOutMsg* msg) {
  Client* client = msg->m_client;
  if (!client) {
    return;
  }

  OutMsg* real_msg = msg->m_outmsg;
  if (!real_msg) {
    return;
  }

  auto real_client = reinterpret_cast<RealClientInfo*>(client->m_opaque);
  if (!real_client->m_ctx_impl) {
    errlog("the client has not been added");
    return;
  }

  if (real_client->m_state != ClientConnState::CONNECTED) {
    dbglog("the client is not connected");
    return;
  }

  MgConn* c = real_client->m_conn;
  if (real_client->IsWritable()) {
    SendClientOutMsg(c, real_msg);
  }
}

void WebServer::OnRecvHTTPRequest(MgConn* c, void* p) {
  auto hm = reinterpret_cast<struct http_message*>(p);
  auto req = convert_mg_http_message(*hm);
  dbglog("http request: %s", req.dump().c_str());

  std::vector<HttpHandler*> handlers;
  Http404Handler handler404;

  {
    std::lock_guard<std::mutex> guard(m_http_handlers_lock);
    handlers = m_http_handlers;
  }

  handlers.push_back(&handler404);

  HttpConn conn(c);

  for (auto handler : handlers) {
    int rc = handler->handle(&conn, req);
    if (rc >= 0) {
      break;
    }
  }
}

void WebServer::OnRecvWSEstablished(MgConn* c) {
  ClientConn& myconn = m_clients[c];
  myconn.m_id = m_conn_counter++;
  myconn.m_conn = c;
  dbglog("the %" PRIu64 "-th client: %p (%p)",
    myconn.m_id, c, &myconn);
  dbglog("current client count: %zu", m_clients.size());
}

void WebServer::OnRecvWSFrame(MgConn* c, void* p) {
  auto wm = reinterpret_cast<struct websocket_message*>(p);
  if (wm->flags & WEBSOCKET_OP_TEXT) {  // text message
    OnRecvWSTextFrame(c, wm);
  } else if (wm->flags & WEBSOCKET_OP_BINARY) {  // binary message
    OnRecvWSBinaryFrame(c, wm);
  }
}

void WebServer::OnRecvWSTextFrame(MgConn* c,
  struct websocket_message* wm) {
  const char* data = reinterpret_cast<char*>(wm->data);
  std::string msg(data, wm->size);
  errlog("received text message from client %p: %s",
    c, msg.c_str());
}

void WebServer::OnRecvWSBinaryFrame(MgConn* c,
  struct websocket_message* wm) {
  // decoding
  msgpack::object_handle oh;
  msgpack::object obj;

  try {
    const char* data = reinterpret_cast<const char*>(wm->data);
    oh = msgpack::unpack(data, wm->size);
    obj = oh.get();
  } catch (std::exception e) {
    errlog("msgpack::unpack failed, data size: %zu bytes", wm->size);
    return;
  }

  // convert
  std::map<std::string, msgpack::object> msg;

  try {
    obj.convert(msg);
  } catch (std::exception e) {
    errlog("malformed client message: not a string-to-object map");
    return;
  }

  // check fields
  if (msg.find("source") == msg.end()) {
    errlog("malformed client message: source not found");
    return;
  }

  if (msg.find("topic") == msg.end()) {
    errlog("malformed client message: topic not found");
    return;
  }

  if (msg.find("data") == msg.end()) {
    errlog("malformed client message: data not found");
    return;
  }

  try {
    const std::string& source = msg["source"].convert();
    const std::string& topic = msg["topic"].convert();
    const std::string& data1 = msg["data"].convert();

    dbglog("receive binary message: "
           "source: %s, topic: %s, data size: %zu",
           source.c_str(), topic.c_str(), data1.size());

    if (topic == "subscribe") {
      OnRecvSubscribeMessage(c, data1);
    } else if (topic == "unsubscribe") {
      OnRecvUnsubscribeMessage(c, data1);
    } else {
      std::vector<Receiver*> receivers;

      {
        std::lock_guard<std::mutex> guard(m_receivers_lock);
        receivers = m_receivers;
      }

      for (auto receiver : receivers) {
        receiver->recv(source.c_str(), topic.c_str(),
          data1.data(), data1.size());
      }
    }
  } catch (std::exception e) {
    errlog("OnRecvWSBinaryFrame: %s", e.what());
    // const char* save_file = "/data/flow_bad_input_msg.dat";
    // write_file(save_file, (const char*)wm->data, wm->size);
  }
}

void WebServer::OnRecvSubscribeMessage(
  MgConn* c, const std::string& data) {
  auto it = m_clients.find(c);
  if (it != m_clients.end()) {
    ClientConn& myconn = it->second;
    myconn.Subscribe(data);
  }
}

void WebServer::OnRecvUnsubscribeMessage(
  MgConn* c, const std::string& data) {
  auto it = m_clients.find(c);
  if (it != m_clients.end()) {
    ClientConn& myconn = it->second;
    myconn.Unsubscribe(data);
  }
}

void WebServer::OnRecvClientDrain(MgConn* c, void* p) {
  ClientConn& myconn = m_clients[c];
  int* num = reinterpret_cast<int*>(p);
  dbglog("the %" PRIu64 "-th client"
    " %p (%p) has just send %d bytes, send_mbuf.len: %zu",
    myconn.m_id, c, &myconn, *num, c->send_mbuf.len);

  auto topics = GetTopicsByClient(myconn);
  CheckTopicList(topics);
}

void WebServer::OnRecvClientClose(MgConn* c) {
  ClientConn& myconn = m_clients[c];
  dbglog("the %" PRIu64 "-th client"
    " %p (%p) has closed", myconn.m_id, c, &myconn);

  auto topics = GetTopicsByClient(myconn);
  m_clients.erase(c);
  dbglog("current client count: %zu",
    m_clients.size());

  CheckTopicList(topics);
}

void WebServer::PushOutMsg(const OutMsg* msg) {
  int rc = send(m_sender_socks->End0(), reinterpret_cast<const char*>(&msg),
    sizeof &msg, 0);
  if (rc == -1) {
    errlog("failed to send message to broker: %s", strerror(errno));
  }
}

void WebServer::SendOutMsg(MgConn* c, OutMsgSP msg) {
  BinRef bin = msg->GetSerializedData();
  mg_send_websocket_frame(c, WEBSOCKET_OP_BINARY, bin.data, bin.size);
  dbglog("send_mbuf of %p: len=%zu, size=%zu",
    c, c->send_mbuf.len, c->send_mbuf.size);
}

void WebServer::PushClientOutMsg(const ClientOutMsg* msg) {
  int rc = send(m_client_socks->End0(), reinterpret_cast<const char*>(&msg),
    sizeof &msg, 0);
  if (rc == -1) {
    errlog("failed to send message to broker: %s", strerror(errno));
    delete msg;
  }
}

void WebServer::SendClientOutMsg(MgConn* c, OutMsg* msg) {
  BinRef bin = msg->GetSerializedData();
  mg_send_websocket_frame(c, WEBSOCKET_OP_BINARY, bin.data, bin.size);
  dbglog("send_mbuf of %p: len=%zu, size=%zu",
    c, c->send_mbuf.len, c->send_mbuf.size);
}

std::vector<MgConn*> WebServer::GetClientsByTopic(const char* topic) const {
  std::vector<MgConn*> clients;

  for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
    MgConn* c = it->first;
    const ClientConn& myconn = it->second;
    bool match = myconn.MatchTopic(topic);
    if (match) {
      clients.push_back(c);
    }
  }

  return clients;
}

bool WebServer::IsAnyClientsDrained(
  const std::vector<MgConn*>& clients) const {
  for (auto it = clients.begin(); it != clients.end(); ++it) {
    MgConn* client = *it;
    if (ClientConn::IsMgConnDrained(client)) {
      return true;
    }
  }

  return false;
}

bool WebServer::IsMostClientsDrained(
  const std::vector<MgConn*>& clients) const {
  size_t total = clients.size();

  size_t drained_count = 0;
  for (auto it = clients.begin(); it != clients.end(); ++it) {
    MgConn* client = *it;
    if (ClientConn::IsMgConnDrained(client)) {
      drained_count += 1;
    }
  }

  if (total <= 2) {
    return drained_count >= total;
  } else {
    return drained_count * 2 > total;
  }
}

void WebServer::CheckTopic(const std::string& topic) {
  auto& queue = m_queues[topic];
  if (queue.size() == 0) {  // no message
    dbglog("queue %s size is 0", topic.c_str());
    return;
  }

  auto clients = GetClientsByTopic(topic.c_str());
  if (clients.size() == 0) {  // no client
    dbglog("there are no clients for topic %s", topic.c_str());
    return;
  }

  if (m_drain_mode == DRAIN_MODE_MOST) {
    bool most_drained = IsMostClientsDrained(clients);
    if (!most_drained) {
      dbglog("topic %s: not most drained", topic.c_str());
      return;
    }
  } else {
    bool any_drained = IsAnyClientsDrained(clients);
    if (!any_drained) {
      dbglog("topic %s: no client drained", topic.c_str());
      return;
    }
  }

  OutMsgSP msg = queue.pop();
  for (auto it = clients.begin(); it != clients.end(); ++it) {
    auto& client = *it;
    if (!ClientConn::IsMgConnDrained(client)) {
      dbglog("topic %s: client not drained", topic.c_str());
      continue;
    }

    SendOutMsg(client, msg);
  }
}

void WebServer::CheckTopicList(const std::vector<std::string>& topics) {
  for (auto it = topics.begin(); it != topics.end(); ++it) {
    const std::string& topic = *it;
    CheckTopic(topic);
  }
}

std::vector<std::string> WebServer::GetTopicsByClient(
  const ClientConn& client) const {
  std::vector<std::string> topics;

  for (auto it = m_queues.begin(); it != m_queues.end(); ++it) {
    auto& topic = it->first;
    if (client.MatchTopic(topic)) {
      topics.push_back(topic);
    }
  }

  return topics;
}

void WebServer::DoClientConnection() {
  int64_t now = get_time_in_msecs_i64();
  if (now - m_clients_conn_time < 1000) {
    return;
  }

  m_clients_conn_time = now;

  std::lock_guard<std::mutex> guard(m_clients_lock);

  for (auto real_client : m_real_clients) {
    if (real_client->m_state != ClientConnState::CLOSED) {
      continue;
    }

    int64_t dt = now - real_client->m_close_time;
    if (dt < 1000) {
      continue;
    }

    m_loop->AddClient(real_client);
  }
}

}  // namespace flow
