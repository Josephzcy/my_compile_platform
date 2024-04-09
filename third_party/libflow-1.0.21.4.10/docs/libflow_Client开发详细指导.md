# 1. Libflow通讯协议介绍及客户端开发指导

<!-- TOC -->

- [1. Libflow通讯协议介绍及客户端开发指导](#1-libflow通讯协议介绍及客户端开发指导)
  - [1.1. Client的开发指导(与Server的交互方式介绍)](#11-client的开发指导与server的交互方式介绍)
    - [1.1.1. 基本介绍](#111-基本介绍)
    - [1.1.2. 订阅topic](#112-订阅topic)
    - [1.1.3. 接收消息](#113-接收消息)
    - [1.1.4. 发送消息](#114-发送消息)
    - [1.1.5. 应用举例](#115-应用举例)

<!-- /TOC -->

## 1.1. Client的开发指导(与Server的交互方式介绍)



### 1.1.1. 基本介绍
&nbsp;&nbsp;&nbsp;&nbsp;
一般情况下，我司提供的相关接口与功能实现成`Server`，并运行在主机上。而使用我司服务/数据的用户的应用程序，相对`Server`而言就是`Client`。  
&nbsp;&nbsp;&nbsp;&nbsp;
本章节将指导用户在实现`Client`的时候，如何与`Server`进行交互。 

- 我司 `Server`会在特定的端口号进行侦听，并提供 [WebSocket](https://tools.ietf.org/html/rfc6455) 服务。
- `Server-Client`之间的通讯使用WebSocket的 [binary data frame](https://tools.ietf.org/html/rfc6455#section-5.6) 来传递消息。Wire protocol为 [MessagePack](https://msgpack.org/) 。
- 每个消息至少包含 `(source, topic, data)` 3个字段，打包成一个MessagePack *map*对象。
    - `source`: 字符串类型，是**发送者的名字**
    - `topic`: 字符串类型，是**消息的主题**（类似于channel name）
    - `data`: 二进制数据，是**消息的内容**
- 采用[Pub-Sub(Publish-Subscribe)](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern) 作为消息传递范式。
  - `Server`可以在多个`topic`上发送消息(Publish)。
  - `Client`可以订阅接收(Subscribe)多个`topic`的消息。
    - 因此`Client`在初始化、成功连接`Server`之后，一般要先发消息给`Server`，告诉`Server`自己订阅的消息的`topic`。

### 1.1.2. 订阅topic
- `Client`需要订阅某个主题时，需要在`特定的topic:"subscribe"`上将需要订阅的topic放在data域段中发给`Server`：
  - 开始收听某个主题: `{source: "client_name", topic: "subscribe", data: "Client需要订阅的topic的名字"}`
- `Client`需要停止订阅某个主题时，需要在`特定的topic:"unsubscribe"`上将需要停止订阅的topic放在data域段中发给`Server`：
  - 停止收听某个主题: `{source: "client_name", topic: "unsubscribe", data: "Client需要停止订阅的topic的名字"}`

- 举例
  1. 名字为`face_id_api_user`的`Client`需要订阅topic为`dms.face_id_api_server`的消息
     - 则应该向`Server`发送消息：
```
{
  source: "face_id_api_user",
  topic:  "subscribe",
  data:   "dms.face_id_api_server"
}
```
  1. 名字为`face_id_api_user`的`Client`需要停止订阅topic为`dms.face_id_api_server`的消息
     - 则应该向`Server`发送消息：
```
{
  source: "face_id_api_user",
  topic:  "unsubscribe",
  data:   "dms.face_id_api_server"
}
```

### 1.1.3. 接收消息
1. `Client`在成功向`Server`订阅指定topic的消息之后，`Server`在有对应消息产生的时候，就会将消息发送给`Client`。
2. `Client`接收到的消息为一串二进制流。
3. 使用msgpack协议解压后，会得到三个字段`(source, topic, data)`的消息：
```
{
  source: "xxx",
  topic:  "消息的topic名字",
  data:   "消息的内容message_sent_from_server"
}
```


### 1.1.4. 发送消息
- `Client`将自己需要发送的消息按照基本介绍中的格式打包好，通过websocket将二进制流发送给`Server`即可。

### 1.1.5. 应用举例
下面，我们使用几段**伪码**来呈现下上述主要过程在程序中的实现方式：  
1. 初始化一个`Client`，并订阅主题为`dms.face_id_api_server`的消息：
```cpp
void StartClient() {
  // 使用 WebSocket 连接到服务器
  WebSocket.connet_to("ws:127.0.0.1:23375");

  // 准备订阅主题 `dms.face_id_api_server` 的报文
    // 填写报文内容
    msg = {
            'source': 'my_client',
            'topic': 'subscribe',
            'data': 'dms.face_id_api_server',
          }
    // 用msgpack协议打包成二进制流
    data = msgpack.packb(msg)

    // 向server发送打包好的二进制流
    WebSocket.send_bytes(data)
}
```

2. `Client`给`Server`在主题`dms.face_id_api_server`上发送一个内容为“Please_start_face_identification”的消息：
```cpp
void SendMsgToServer() {
  // 向Server发送主题 `dms.face_id_api_server` 的消息
  // 消息内容为： Please_start_face_identification
    // 填写消息内容
    msg = {
            'source': 'my_client',
            'topic': 'dms.face_id_api_server',
            'data': 'Please_start_face_identification', 
          }
    // 用msgpack协议打包成二进制流
    data = msgpack.packb(msg)

    // 向server发送打包好的二进制流
    WebSocket.send_bytes(data)
}
```

3. `Client`收到`Server`发送过来的字节流，并进行解析：
```cpp
// recv_data 是Server发送过来的字节流
void OnReceiveMsgFromServer(BinaryStream recv_data) {
  // 先用msgpack协议将接收到的二进制流解压
  recv_msg = msgpack.unpackb(recv_data)

  // 获取各个字段的内容
  msg_topic = recv_msg.topic;
  msg_content = recv_msg.data;
}

```