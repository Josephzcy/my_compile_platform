// Copyright 2017 minieye.cc
// Author: Zhang Qi <zhangqi@minieye.cc>
// Date: 2017-12-25

#ifndef _FLOW_H_
#define _FLOW_H_

#if _pragma_once_support
#pragma once
#endif

#include <map>
#include <vector>
#include <utility>
#include <string>
#include <sstream>

namespace flow {

template <typename T>
std::string stringify(const T& thing) {
  std::ostringstream os;
  os << thing;
  return os.str();
}

typedef std::pair<std::string, std::string> StrPair;
typedef std::vector<StrPair> StrPairVec;
typedef std::map<std::string, std::string> StrMap;
typedef std::map<std::string, std::string> Config;

class Context;

class Sender {
 public:
  Sender(Context* ctx, const char* name);
  ~Sender();

  void send(const std::string& topic,  // '\0' terminated string
            const char* data,          // any binary data
            size_t size);              // < 2^32

  void send(const char* topic,  // '\0' terminated string
            const char* data,   // any binary data
            size_t size) {      // < 2^32
    send(std::string(topic), data, size);
  }

  void send(const std::string& topic,
            const std::string& data) {
    send(topic, data.data(), data.size());
  }

  template <typename DataType>
  void send(const std::string& topic,
            const DataType& data) {
    auto&& s_data = stringify(data);
    send(topic, s_data.data(), s_data.size());
  }

  template <typename TopicType, typename DataType>
  void send(TopicType topic, DataType data) {
    auto s_topic = stringify(topic);
    auto s_data = stringify(data);
    send(s_topic, s_data.data(), s_data.size());
  }

 private:
  void* m_opaque;
};

class Receiver {
 public:
  Receiver();
  virtual ~Receiver();

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

class HttpConn {
 public:
  void send_response(const HttpResponse& res);

  // Get the underlying socket file descriptor.
  int get_socket() const;

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

 private:
  void* m_opaque;
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

  void send(const char* topic,  // '\0' terminated string
            const char* data,   // any binary data
            size_t size) {      // < 2^32
    send(std::string(topic), data, size);
  }

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

 private:
  void* m_opaque;
};

extern Context ctx0;

extern const char* version;

}  // namespace flow

#endif
