#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <chrono>
#include <thread>
#include "flow.hpp"

#if defined(__APPLE__)
#define flow_pthread_setname(name)  pthread_setname_np(name)
#elif defined(__FreeBSD__)
#define flow_pthread_setname(name)  pthread_set_name_np(pthread_self(), name)
#else
#define flow_pthread_setname(name)  pthread_setname_np(pthread_self(), name)
#endif

class MyReceiver : public flow::Receiver {
  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size) {       // < 2^32
    fprintf(stderr, "MyReceiver::recv(%s, %s, %s)\n",
      source, topic, std::string(data, size).c_str());
  }
};

static void sleep_ms(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void func_th1() {
  flow_pthread_setname("thread_1");

  sleep_ms(2000);

  flow::Sender sender(&flow::ctx0, "th1");

  for (int i = 0; i < 10; i += 1) {
    const char* text = "Hello, world!";
    sender.send("test.foo", text, strlen(text));
    sleep_ms(1200);
  }

  fprintf(stderr, "the th1 thread is done\n");
}

void func_th2() {
  flow_pthread_setname("thread_2");

  sleep_ms(2000);

  flow::Sender sender(&flow::ctx0, "th2");

  for (int i = 0; i < 10; i += 1) {
    const char* text = "Are you OK?";
    sender.send("test.bar", text, strlen(text));
    sleep_ms(800);
  }

  fprintf(stderr, "the th2 thread is done\n");
}

void func_th3() {
  flow_pthread_setname("thread_3");

  sleep_ms(2000);

  flow::Sender sender(&flow::ctx0, "th3");

  for (int i = 0; i < 10; i += 1) {
    const char* text = "\x01\x02\x03\x04\x05\x06\x07\x08";
    sender.send("test1.bar", text, strlen(text));
    sleep_ms(800);
  }

  fprintf(stderr, "the th3 thread is done\n");
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
    exit(1);
  }

  printf("Listening on %s:%s\n", host, port);

  typedef void (*thread_func_t)(void);
  thread_func_t funcs[3] = {
    func_th1, func_th2, func_th3,
  };

  std::thread threads[3];

  for (int i = 0; i < 3; i += 1) {
    threads[i] = std::thread(funcs[i]);
  }

  for (int i = 0; i < 3; i += 1) {
    threads[i].join();
  }

  return 0;
}
