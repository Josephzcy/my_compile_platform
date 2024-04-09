
/* Copyright [2018] <xuzhixiang@minieye.cc> */

#include "flow.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>

class LibflowServerExample : public flow::Receiver {
 private:
  const std::string kMinieyeVehicleInfoTopic = "MINIEYE.VehicleInfo.v1";
  flow::Context flow_context;
  flow::Sender flow_sender;

 public:
  LibflowServerExample()
        : flow_context({
            {"addr", "0.0.0.0"},
            {"port", "23666"},
            {"queue." + kMinieyeVehicleInfoTopic + ".capacity", "1"}
          })
        , flow_sender(&flow_context, "MinieyeVehicleInfoSender") {
    flow_context.add_receiver(this);
    auto ret = flow_context.start();
    if (ret != 0) {
      std::cerr << "flow_context start failed." << std::endl;
      exit(-1);
    }
  }

  ~LibflowServerExample() {
    flow_context.stop();
  }

  void Send(std::string str_to_send) {
    flow_sender.send(kMinieyeVehicleInfoTopic, str_to_send.data(), str_to_send.length());
    std::cout << "LibflowServer::Send: " << str_to_send << "\n";
  }

  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size) {       // < 2^32
    fprintf(stderr, "LibflowServer::recv(%s, %s, %s)\n",
      source, topic, std::string(data, size).c_str());
  }
};

class LibflowClientExample : public flow::Client {
 public:
  explicit LibflowClientExample(const flow::ClientConfig& config) : flow::Client(config) {}

  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size) {       // < 2^32
    fprintf(stderr, "LibflowClient::recv(%s, %s, %s)\n",
      source, topic, std::string(data, size).c_str());
  }

  void Send(std::string str_to_send) {
    this->send(std::string("ClientRespTopic").c_str(), str_to_send.data(), str_to_send.length());
    std::cout << "LibflowClient::Send: " << str_to_send << "\n";
  }
};


int main(int argc, char *argv[]) {
  // Client
  flow::ClientConfig client_config = {
    "LibflowStringReceiver",  // client name
    "ws://127.0.0.1:23666",   // server address
    "*",                      // topics to subscribe. ("*" means subscribe all topics.)
  };
  LibflowClientExample client(client_config);

  // 每个Sever/Client都必须被一个flow::Context管理
  // flow::Context使用flow::Config初始化，
  // flow::Config会指定这个context管理的是Server还是Client，若是Server，则需使用ip&端口号等信息初始化。
  flow::Config  client_ctx_config {{"servers", "none"}};  // disable the server
  flow::Context client_ctx(client_ctx_config);
  client_ctx.add_client(&client); // 将上面初始化完成的client注册到这个flow::Context中
  if (client_ctx.start() != 0) { // 启动flow::Context中的所有Client
    fprintf(stderr, "Error: cannot start flow context\n");
    exit(1);
  }

  // Server
  LibflowServerExample server;

  sleep(1);

  // Server send to client
  for (uint i = 0; i < 5; i++) {
    char c_str[300];
    snprintf(c_str, sizeof(c_str), "(This is msg from server: %d)", i);
    server.Send(std::string(c_str));
    sleep(1);
  }

  // Client send to server
  for (uint i = 0; i < 5; i++) {
    char c_str[300];
    snprintf(c_str, sizeof(c_str), "(This is msg from client: %d)", i);
    client.Send(std::string(c_str));
    sleep(1);
  }

  client_ctx.stop();
  return 0;
}
