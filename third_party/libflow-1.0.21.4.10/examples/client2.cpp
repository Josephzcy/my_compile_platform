#include <stdio.h>
#include <string.h>
#include <string>
#include <chrono>
#include <thread>
#include "flow.hpp"

class MyClient : public flow::Client {
 public:
  MyClient(const flow::ClientConfig& config) :
    flow::Client(config) {
  }

  // This function will be run in an event loop.
  // DO NOT do blocking I/O or heavy computation in this function.
  // The data will be dropped after calling this function. If you
  // want to use it in future time, make your own copy.
  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size) {       // < 2^32
    printf("--------------------------------------------------\n");
    fprintf(stderr, "MyClient::recv(%s, %s, %s)\n",
      source, topic, std::string(data, size).c_str());
  }
};

int main(int argc, char* argv[]) {
  flow::Config ctx_config {{"servers", "none"}};
  flow::ctx0 = flow::Context(ctx_config);

  std::string uri = "ws://127.0.0.1:24012";
  flow::ClientConfig config = {"FlowClient", uri, "*"};

  MyClient client(config);
  flow::ctx0.add_client(&client);

  if (flow::ctx0.start() != 0) {
    fprintf(stderr, "Error: cannot start flow context\n");
    exit(1);
  }

  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
