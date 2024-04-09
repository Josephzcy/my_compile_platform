// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#ifndef _FLOW_INNER_H_
#define _FLOW_INNER_H_

#if _pragma_once_support
#pragma once
#endif

#include <mongoose.h>
#include <map>
#include <deque>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <thread>  // NOLINT(build/c++11)
#include <mutex>   // NOLINT(build/c++11)
#include "./queue.hpp"

namespace flow {

typedef std::pair<std::string, std::string> StrPair;
typedef std::vector<StrPair> StrPairVec;
typedef std::map<std::string, std::string> StrMap;
typedef std::map<std::string, std::string> Config;

class Context;
class ContextImpl;
class SenderImpl;

class Sender {
 public:
  Sender(Context* ctx, const char* name);
  ~Sender();

  void send(const std::string& topic,  // '\0' terminated string
            const char*        data,   // any binary data
            size_t             size);  // < 2^32

 private:
  SenderImpl* m_impl;
};

class Receiver {
 public:
  Receiver();                        // automatically add()
  virtual ~Receiver();               // automatically remove()

  // This function will be run in an event loop.
  // DO NOT do blocking I/O or heavy computation in this function.
  // The data will be dropped after calling this function. If you
  // want to use it in future time, make your own copy.
  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size) = 0;    // < 2^32
};

struct HttpRequest {
  std::string proto;
  std::string method;
  std::string uri;
  std::string query;
  std::string headers;
  std::string body;

  // methods
  std::string get_url() const;
  std::string dump() const;
};

struct HttpResponse {
  int code;
  std::string headers;
  std::string body;

  // methods
  HttpResponse();
  explicit HttpResponse(int c);
  HttpResponse(int c, const std::string& h, const std::string& b);
  HttpResponse(int c, const StrPairVec& h, const std::string& b);
  HttpResponse& operator=(const HttpResponse& res) = default;
  std::string dump() const;
  static std::string get_status(int code);
};

StrPairVec decode_query_vp(const std::string& str);
StrMap decode_query_map(const std::string& str);

StrPairVec decode_headers_vp(const std::string& str);
StrMap decode_headers_map(const std::string& str);

std::string encode_query_vp(const StrPairVec& o);
std::string encode_query_map(const StrMap& o);

std::string encode_headers_vp(const StrPairVec& o);
std::string encode_headers_map(const StrMap& o);

std::string get_status_line(int code);

class HttpConn {
 public:
  explicit HttpConn(mg_connection* conn);

  // Get the underlying socket file descriptor.
  int get_socket() const;

  void send_response(const HttpResponse& res);

  // Sends the response line and headers.
  // This function sends the response line with the `status_code`,
  // and automatically sends one header:
  // either "Content-Length" or "Transfer-Encoding".
  // If `content_length` is negative, then "Transfer-Encoding: chunked" header
  // is sent, otherwise, "Content-Length" header is sent.
  //
  // NOTE: If `Transfer-Encoding` is `chunked`, then message body must be sent
  // using `send_chunk()`. Otherwise, `send_body()` must be used.
  // Extra headers could be set through `extra_headers`.
  // Note `extra_headers` must NOT be terminated by a "\r\n".
  void send_head(int status_code, int64_t content_length,
    const std::string& extra_headers);

  void send_body(const char* data, size_t size);

  void send_trunk(const char* data, size_t size);

  void close();

 public:
  mg_connection* m_conn;
};

class HttpHandler {
 public:
  HttpHandler();
  virtual ~HttpHandler();

  // Handle every http request.
  // Return value:
  //  -1: I'm not interested in this request
  //   0: the request has been processed
  //   1: the request will be processed later
  // This function will be run in an event loop.
  // DO NOT do blocking I/O or heavy computation in this function.
  virtual int handle(HttpConn* conn, const HttpRequest& req) = 0;
};

class Http404Handler : public HttpHandler {
  virtual int handle(HttpConn* conn, const HttpRequest& req);
};

struct ClientConfig {
  std::string name;    // client name, e.g. "FlowClient"
  std::string url;     // server address, e.g. "ws://127.0.0.1:24012"
  std::string topics;  // e.g. "foo.bar, x.y.*"
};

class Client {
 public:
  explicit Client(const ClientConfig& config);
  virtual ~Client();

  void send(const std::string& topic,  // '\0' terminated string
            const char* data,          // any binary data
            size_t size);              // < 2^32

  // This function will be run in an event loop.
  // DO NOT do blocking I/O or heavy computation in this function.
  // The data will be dropped after calling this function. If you
  // want to use it in future time, make your own copy.
  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size);        // < 2^32

 public:
  void* m_opaque;
};

class Context {
 public:
  Context();
  explicit Context(const Config& config);
  ~Context();

  const Context& operator=(Context&& ctx);

  int start();  // 0: success, otherwise: error
  void stop();
  void add_receiver(Receiver* receiver);
  void remove_receiver(Receiver* receiver);
  void add_http_handler(HttpHandler* handler);
  void remove_http_handler(HttpHandler* handler);
  void add_client(Client* client);
  void remove_client(Client* client);

 public:
  ContextImpl* m_impl;
};

extern Context ctx0;

extern const char* version;

////////////////////////////////////////////////////////////

typedef struct mg_mgr MgMgr;
typedef struct mg_connection MgConn;
typedef void (*MgEventHandler)(MgConn *c, int ev, void *p);

struct BinRef {
  const void* data;
  size_t      size;
};

class OutMsg {
 public:
  OutMsg();
  virtual ~OutMsg();

  virtual void Init(const std::string& source,  // '\0' terminated string
                    const std::string& topic,   // '\0' terminated string
                    const char*        data,    // any binary data
                    size_t             size,    // <= 2^32
                    int64_t            time);   // micro-seconds from epoch
  virtual const std::string& GetTopic() const;
  virtual BinRef GetSerializedData() const;

 protected:
  std::string m_topic;
  void* m_opaque;
};

class OutMsgRaw : public OutMsg {
 public:
  virtual void Init(const std::string& source,  // '\0' terminated string
                    const std::string& topic,   // '\0' terminated string
                    const char*        data,    // any binary data
                    size_t             size,    // <= 2^32
                    int64_t            time) override;  // micro-seconds from epoch
  virtual BinRef GetSerializedData() const override;

 protected:
  std::vector<char> m_data;
};

typedef std::shared_ptr<const OutMsg> OutMsgSP;
typedef Queue<OutMsgSP> OutMsgQueue;

class ClientOutMsg {
 public:
  ClientOutMsg();
  ClientOutMsg(Client* client, OutMsg* outmsg);
  ~ClientOutMsg();

  Client* m_client;
  OutMsg* m_outmsg;
};

class SenderImpl {
 public:
  SenderImpl(Context* ctx, const char* name);
  SenderImpl(ContextImpl* ctx_impl, const char* name);

  void send(const std::string& topic, const char* data, size_t size);

 private:
  ContextImpl* m_context_impl;
  const std::string m_name;
};

class ClientConn {
 public:
  void Subscribe(const std::string& topic);
  void Unsubscribe(const std::string& topic);
  bool MatchTopic(const std::string& topic) const;
  bool IsDrained() const;
  static bool IsMgConnDrained(const MgConn* conn);

 public:
  uint64_t m_id;
  MgConn* m_conn;
  std::vector<std::string> m_topics;
};

enum class ClientConnState {
  CLOSED,
  CONNECTING,
  CONNECTED,
};

class RealClientInfo {
 public:
  RealClientInfo() :
    m_client(nullptr),
    m_ctx_impl(nullptr),
    m_conn(nullptr),
    m_close_time(0),
    m_state(ClientConnState::CLOSED)
    {}

  void HandleClientEvent(MgConn* c, int ev, void* p);
  void HandleWebsocketMessage(int opcode, char* data, size_t size);
  void DoSubscribe();
  bool IsWritable() const;

 public:
  ClientConfig m_config;
  Client* m_client;
  ContextImpl* m_ctx_impl;
  MgConn* m_conn;
  uint64_t m_close_time;
  ClientConnState m_state;  // 0: closed, 1: connecting, 2: connected
};

class SockPair {
 public:
  SockPair();
  explicit SockPair(int sock_type);
  ~SockPair();

  sock_t& End0();
  sock_t& End1();

 private:
  sock_t m_socks[2];
};

struct ServerInfo {
  std::string    listen_addr;
  MgEventHandler handler;
  void*          user_data;
  MgConn*        connection;
};

class WebServer;

class EventLoop {
 public:
  explicit EventLoop(WebServer* server);
  ~EventLoop();
  void Run();
  void Stop();
  int AddHttpServer(const std::string& listen_addr,
    MgEventHandler handler, void* user_data);
  int AddPlainSocket(const sock_t& sock,
    MgEventHandler handler, void* user_data);
  void AddClient(RealClientInfo* real_client);

 private:
  bool m_running;
  WebServer* m_server;
  MgMgr m_mg_mgr;
  std::vector<ServerInfo> m_servers;
};

const int DRAIN_MODE_ANY = 1;
const int DRAIN_MODE_MOST = 2;

const size_t CLIENT_SEND_MBUF_LEN_MAX = 1024 * 1024 * 10;

class WebServer {
 public:
  explicit WebServer(ContextImpl* ctx_impl);
  WebServer(ContextImpl* ctx_impl, const Config& config);
  virtual ~WebServer();

  // You can add/remove a receiver even when the server is stopped.
  void add_receiver(Receiver* receiver);
  void remove_receiver(Receiver* receiver);
  void add_http_handler(HttpHandler* handler);
  void remove_http_handler(HttpHandler* handler);
  void add_client(Client* client);
  void remove_client(Client* client);

  int GetQueueCapacityConfig(const std::string& topic);
  // 0: success, otherwise: 1
  int SetQueueCapacityConfig(const std::string& topic, int capacity);

  int Start();
  void Run();

  void HandleHttpEvent(MgConn* c, int ev, void* p);
  void HandleSenderSockEvent(MgConn* c, int ev, void* p);
  void HandleClientSockEvent(MgConn* c, int ev, void* p);
  void HandleClientEvent(MgConn* c, int ev, void* p);
  void HandleSenderSockRecvEvent(MgConn* c);
  void HandleClientSockRecvEvent(MgConn* c);
  void OnRecvContextMessage(const std::string& topic, const OutMsg* msg);
  void OnRecvOutputMessage(const std::string& topic, const OutMsg* msg);
  void OnRecvClientOutputMessage(const ClientOutMsg* msg);
  void OnRecvHTTPRequest(MgConn* c, void* p);
  void OnRecvWSEstablished(MgConn* c);
  void OnRecvWSFrame(MgConn* c, void* p);
  void OnRecvWSTextFrame(MgConn* c, struct websocket_message* wm);
  void OnRecvWSBinaryFrame(MgConn* c, struct websocket_message* wm);
  void OnRecvSubscribeMessage(MgConn* c, const std::string& data);
  void OnRecvUnsubscribeMessage(MgConn* c, const std::string& data);
  void OnRecvClientDrain(MgConn* c, void* p);
  void OnRecvClientClose(MgConn* c);

  void PushOutMsg(const OutMsg* msg);
  void SendOutMsg(MgConn* client, OutMsgSP msg);

  void PushClientOutMsg(const ClientOutMsg* msg);
  void SendClientOutMsg(MgConn* client, OutMsg* msg);

  std::vector<MgConn*> GetClientsByTopic(const char* topic) const;
  bool IsAnyClientsDrained(const std::vector<MgConn*>& clients) const;
  bool IsMostClientsDrained(const std::vector<MgConn*>& clients) const;

  std::vector<std::string> GetTopicsByClient(const ClientConn& client) const;

  void CheckTopic(const std::string& topic);
  void CheckTopicList(const std::vector<std::string>& topic);

  void DoClientConnection();

 protected:
  std::thread m_thread;
  ContextImpl* m_ctx_impl;
  Config m_config;
  SockPair* m_sender_socks;
  SockPair* m_client_socks;
  EventLoop* m_loop;
  int m_drain_mode;
  uint64_t m_conn_counter;
  int64_t m_clients_conn_time;
  std::map<MgConn*, ClientConn> m_clients;
  std::map<std::string, OutMsgQueue> m_queues;
  std::vector<Receiver*> m_receivers;
  std::vector<HttpHandler*> m_http_handlers;
  std::vector<RealClientInfo*> m_real_clients;
  std::mutex m_receivers_lock;
  std::mutex m_http_handlers_lock;
  std::mutex m_clients_lock;
};

class ContextImpl {
 public:
  ContextImpl();
  explicit ContextImpl(const Config& config);
  ~ContextImpl();

  int start();  // 0: success, otherwise: error
  void stop();
  void add_receiver(Receiver* receiver);
  void remove_receiver(Receiver* receiver);
  void add_http_handler(HttpHandler* handler);
  void remove_http_handler(HttpHandler* handler);
  void add_client(Client* client);
  void remove_client(Client* client);

  void init_log_level();

 public:
  bool m_started;
  Config m_config;
  std::unique_ptr<WebServer> m_server;
};

}  // namespace flow

#endif
