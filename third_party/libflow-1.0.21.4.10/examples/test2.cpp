#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <chrono>
#include <thread>
#include "flow.hpp"

static void sleep_ms(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

class MyReceiver : public flow::Receiver {
  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size) {       // < 2^32
    fprintf(stderr, "MyReceiver::recv(%s, %s, %s)\n",
      source, topic, std::string(data, size).c_str());
  }
};

void thread_func() {
  flow::Sender sender(&flow::ctx0, "th1");

  for (int i = 0; ; i += 1) {
    char buf[128];
    snprintf(buf, sizeof(buf), "There are %d sheep.", i);
    printf("SEND topic: test.foo, data: %s\n", buf);
    sender.send("test.foo", buf, strlen(buf));
    sleep_ms(1000);
  }

  fprintf(stderr, "the thread is done\n");
}

int main() {
  const char* host = "127.0.0.1";
  const char* port = "24012";

  flow::ctx0 = flow::Context({
    {"addr", host},
    {"port", port},
    {"queue.test.foo.capacity", "2"},
  });

  auto receiver = new MyReceiver;
  flow::ctx0.add_receiver(receiver);

  if (flow::ctx0.start() != 0) {
    fprintf(stderr, "Error: cannot start flow context\n");
    exit(1);
  }

  printf("Listening on %s:%s\n", host, port);

  std::thread th(thread_func);
  th.join();

  return 0;
}
