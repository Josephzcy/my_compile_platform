# libflow

Libflow是一个跨平台的 [IPC](https://en.wikipedia.org/wiki/Inter-process_communication) 库。
它可以在应用程序中添加一个 [WebSocket](https://en.wikipedia.org/wiki/WebSocket) 端口，用于输入或输出任意消息。

Libflow采用 [Pub-Sub](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern) 作为消息传递范式。

## How to Build

```shell
$ ./build.sh <platform> [build_type] [target]

Platform:
  windows              PC (Windows, amd64/x86_64)
  linux                PC (Linux, amd64/x86_64)
  mac                  Apple (macOS, amd64/x86_64)
  m4                   M3, M4 (Android, arm64/armv8)
  x1                   X1 (Linux, arm/armv7)
  c1disp               C1 Display (Android, arm/armv7)

Build type:
  release (default)
  debug

Target:
  all (default)
```

#### Build static or dynamic library

如果你想编译静态库(Linux/Mac: `libflow.a`, Windows: `libflow.lib`): 修改 `src/CMakeLists.txt`，改为 `ADD_LIBRARY(flow STATIC ...)`

如果你想编译动态库(Linux: `libflow.so`, Mac: `libflow.dylib`, Windows: `libflow.dll`): 修改 `src/CMakeLists.txt`，改为 `ADD_LIBRARY(flow SHARED ...)`

#### Build for Ubuntu PC (`Linux x86_64`)

```shell
./build.sh linux
```

编译目录为 `linux_Release`, 可执行文件在 `bin/` 子目录下，库文件在 `lib/` 子目录下。

#### Build for MINIEYE X1 (`Linux armv7`)

```shell
./build.sh x1
```

编译目录为 `x1_Release`.

#### Build for MINIEYE M4 (`Android armv8`)

执行 `build.sh` 前，
1. 从Google Android官方网站下载 Android NDK **r11c**，解压到任意位置。
2. 设定环境变量 `ANDROID_NDK` 为 `android-ndk-r11c` 所在目录。

```shell
export ANDROID_NDK=~/opt/android-ndk-r11c
./build.sh m4
```

编译目录为 `m4_Release`.

#### Build for MINIEYE C1 Display (`Android armv7`)

执行 `build.sh` 前，
1. 从Google Android官方网站下载 Android NDK **r19c**，解压到任意位置。
2. 设定环境变量 `ANDROID_NDK` 为 `android-ndk-r19c` 所在目录。

```shell
export ANDROID_NDK=~/opt/android-ndk-r19c
./build.sh c1disp
```

编译目录为 `c1disp_Release`.

## Protocol

- libflow默认监听 `127.0.0.1:24012` 端口，提供 [WebSocket](https://tools.ietf.org/html/rfc6455) 服务。
- libflow使用WebSocket的 [binary data frame](https://tools.ietf.org/html/rfc6455#section-5.6) 来传递消息。Wire protocol为 [MessagePack](https://msgpack.org/) 。
- 每个消息至少包含 `(source, topic, data)` 3个字段，打包成一个MessagePack *map*对象。
    - `source`: 字符串类型，是**发送者的名字**
    - `topic`: 字符串类型，是**消息的主题**（类似于channel name）
    - `data`: 二进制数据，是**消息的内容**
- 宿主进程中的每个线程可以发送多种主题的消息，也可以接收多种主题的消息。
    - 发送消息使用 *Sender* 类，创建一个此类的实例，调用 `send` 方法即可。*Sender*是线程安全的，`send`方法可以在任意线程中执行。
    - 接收消息使用 *Receiver* 类，继承此类，实现 `recv` 方法，然后调用 `Context::add_receiver` 添加一个实例即可。`recv`是一个回调函数，在libflow的消息循环线程中执行的，不允许阻塞或者执行计算密集型任务。典型的 `recv` 函数应当在 10ms 之内返回。
- 每个客户端可以收听多种主题的消息，也可以发送多种主题的消息。
    - 开始收听某个主题: `{source: "client_name", topic: "subscribe", data: "some_topic"}`
    - 停止收听某个主题: `{source: "client_name", topic: "unsubscribe", data: "some_topic"}`

## Usage

### How to use the Sender

```c++
#include <stdio.h>
#include <flow.hpp>

void* MyThread(void* data) {
  // create a sender
  flow::Sender sender(&flow::ctx0, "MyThread");

  // prepare the message
  const char* my_topic = "my_topic";
  char my_data[1024] = {0};

  // send the message
  sender.send(my_topic, my_data, sizeof my_data);
}

int main() {
  int rc = flow::ctx0.start();
  if (rc != 0) {
    fprintf(stderr, "Error: cannot start flow context\n");
    exit(1);
  }

  flow::ctx0.stop();

  return 0;
}
```

### How to use the Receiver

```c++
#include <stdio.h>
#include <flow.hpp>

class MyReceiver : public flow::Receiver {
  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size) {       // < 2^32
    printf("MyReceiver::recv(%s, %s, %s)\n",
      source, topic, std::string(data, size).c_str());
  }
};

int main() {
  MyReceiver receiver;
  flow::ctx0.add_receiver(&receiver);

  int rc = flow::ctx0.start();
  if (rc != 0) {
    fprintf(stderr, "Error: cannot start flow context\n");
    exit(1);
  }

  flow::ctx0.remove_receiver(&receiver);
  flow::ctx0.stop();
  
  return 0;
}
```

### How to use the Client

```c++
class MyClient : public flow::Client {
 public:
  MyClient(const flow::ClientConfig& config) : flow::Client(config) {}

  virtual void recv(const char* source,  // '\0' terminated string
                    const char* topic,   // any binary data
                    const char* data,    // any binary data
                    size_t size) {       // < 2^32
    fprintf(stderr, "MyClient::recv(%s, %s, %s)\n",
      source, topic, std::string(data, size).c_str());
  }
};

int main() {
  flow::Config ctx_config {{"servers", "none"}};  // disable the server
  flow::ctx0 = flow::Context(ctx_config);

  std::string uri = "ws://127.0.0.1:24012";
  flow::ClientConfig config = {
    "FlowClient",  // client name
    uri,           // server address
    "*",           // topics to subscribe
  };

  MyClient client(config);
  flow::ctx0.add_client(&client);

  if (flow::ctx0.start() != 0) {
    fprintf(stderr, "Error: cannot start flow context\n");
    exit(1);
  }

  // ...

  flow::ctx0.stop();
  return 0;
}
```

### Use custom flow::Context

```c++
#include <stdio.h>
#include <flow.hpp>

flow::Context g_flow({
  {"addr", "127.0.0.1"},
  {"port", "24012"},
});

void* MyThread(void* data) {
  // create a sender
  flow::Sender sender(&g_flow, "MyThread");

  // prepare the message
  const char* my_topic = "my_topic";
  char my_data[1024] = {0};

  // send the message
  sender.send(my_topic, my_data, sizeof my_data);
}

int main() {
  int rc = g_flow.start();
  if (rc != 0) {
    fprintf(stderr, "Error: cannot start flow context\n");
    exit(1);
  }

  g_flow.stop();

  return 0;
}
```

### Configure the default flow::Context

例如: libflow服务端的默认端口是24012，我们可以用如下方法把端口设为24011:

```c++
int main() {
  // ...

  flow::Config ctx_config {
    {"addr", "127.0.0.1"},
    {"port", "24011"},
  };

  flow::ctx0 = flow::Context(ctx_config);

  // ...
}
```

或者

```c++

  flow::ctx0 = flow::Context({
    {"addr", "127.0.0.1"},
    {"port", "24011"},
  });

```

### Configurations

- **addr**: Server监听地址, 默认值: "127.0.0.1"
- **port**: 监听端口, 默认值: "24012"
- **servers**: 设为 "none" 可以关闭服务
- **pusher.socket.type**: 内部线程间通信方式，默认值: "tcp"，也可以设为"udp"
- **sender.serialize**: 消息序列化格式，默认值: "msgpack"；设为"raw"则不做序列化，直接将原始数据发给客户端。服务端和客户端必须将此配置项设为相同值。
- **queue.*topic*.capacity**: 消息队列容量，默认值: "8"。其中 *topic* 为具体发送的消息主题，每个 *topic* 可以设置不同的队列容量。较小的队列容量意味着低延迟、高丢包率，较大的队列容量意味着高延迟、低丢包率。

### flowout

examples/flowout 是一个Go语言编写的libflow客户端，可以接收libflow发出来的所有消息，并打印到终端，或者保存到文件，为调试libflow应用程序提供帮助。

用法: `flowout -server 127.0.0.1:24012`, 即可打印出24012端口发出的所有消息。更多参数请使用 `-help` 查看。

下载地址: <https://flow.minieye.tech/flow/document/view/1569488634fuM8hp>

