#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include <chrono>
#include "flow.hpp"

#if defined(__APPLE__)
#define set_thread_name(name) pthread_setname_np(name)
#elif defined(__FreeBSD__)
#define set_thread_name(name) pthread_set_name_np(pthread_self(), name)
#else
#define set_thread_name(name) pthread_setname_np(pthread_self(), name)
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

class MyHttpHandler : public flow::HttpHandler {
  virtual int handle(flow::HttpConn* conn, const flow::HttpRequest& req) {
    if (req.uri != "/test") {
      return -1;
    }

    flow::HttpResponse res {
      200,
      {
        {"Content-Type", "text/plain"},
        {"X-Handler", "MyHttpHandler"},
      },
      "MyHttpHandler works!",
    };

    conn->send_response(res);
    conn->close();
    return 0;
  }
};

class MyHttpHandler2 : public flow::HttpHandler {
  virtual int handle(flow::HttpConn* conn, const flow::HttpRequest& req) {
    if (req.uri != "/test2") {
      return -1;
    }

    pid_t pid = fork();
    if (pid) {  // parent
      return 1;
    } else {  // child
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));

      flow::HttpResponse res {
        200,
        {
          {"Content-Type", "text/plain"},
          {"X-Handler", "MyHttpHandler"},
        },
        "MyHttpHandler2 works!",
      };

      conn->send_response(res);
      conn->close();
      return 0;
    }
  }
};

void func_th1() {
  set_thread_name("thread_1");

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  flow::Sender sender(&flow::ctx0, "th1");

  for (int i = 0; i < 1000; i += 1) {
    const char* text = "hello, dude";
    sender.send("test.foo", text, strlen(text));
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  fprintf(stderr, "the th1 thread is done\n");
}

int main() {
  auto receiver = new MyReceiver;
  flow::ctx0.add_receiver(receiver);

  auto http_handler = new MyHttpHandler;
  flow::ctx0.add_http_handler(http_handler);

  auto http_handler2 = new MyHttpHandler2;
  flow::ctx0.add_http_handler(http_handler2);

  if (flow::ctx0.start() != 0) {
    exit(1);
  }

  std::thread th1(func_th1);
  th1.join();

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  return 0;
}
