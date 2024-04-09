# FlowClient for Java

这是一个Java语言的libflow客户端。

## 编译

```shell
mvn clean package
```

Maven会自动安装依赖并编译。

## 试运行

这里提供了一个简单的App类来运行FlowClient。

```shell
mvn exec:java -Dexec.mainClass=cc.minieye.software.App
```

## FlowClient用法

### 建立连接

首先，你需要编写一个类，实现 `FlowMessageHandler` 接口。例如：

```java
public class SimpleMessageHandler implements FlowMessageHandler {

	public void recv(FlowMessage msg) {
		System.out.println(msg);
		System.out.printf("Data: %s\n", new String(msg.getData()));
	}

}
```

然后创建一个 `FlowClient` 实例，设置handler，再启动它：

```java
FlowClient c = new FlowClient();  // use default config
c.setHandler(new SimpleMessageHandler());
c.start();
```

如果你没有调用 `setHandler`，那么FlowClient会使用默认的SimpleMessageHandler，它会把接收到的消息输出到stdout。

### 配置FlowClient

```java
FlowClient c = new FlowClient(
  "ws://127.0.0.1:24012/",  // libflow服务端的地址
  "JavaClient",             // 客户端的名称，可以随意设置
  "*"                       // 要收听的消息主题，"*"表示收听全部消息
);
```

消息主题的格式如下：
* 如果只收听一个主题：`topic_1`
* 如果收听多个主题: `topic_1,topic_2,topic_3`
* 支持通配符: 例如 `warn.*` 将同时收听 `warn.aaa`, `warn.bbb`, `warn.xyz` 等等
* 以上2个特性可以同时使用: 例如 `warn.*,error.*,info,*.log`
* 收听全部消息: `*`

### 自动重连

当WebSocket连接因为某种原因（比如服务端挂了）断开后，FlowClient会一直尝试重连。

默认的重连间隔是5秒。可以用如下方法设置重连间隔：

```java
c.setReconnectTime(30 * 1000);  // 30 seconds
```

### 断开连接

当你不想再接受消息时，可以用如下方法断开WebSocket连接：

```java
c.close();
```

调用此方法后，FlowClient将关闭连接，并且不再重连。

## 应用示例
src/test/java/cc/minieye/software/FlowClientTest.java展示了具体的FlowClient使用方法，具体使用时需重新指定libflow服务器的地址和端口。


