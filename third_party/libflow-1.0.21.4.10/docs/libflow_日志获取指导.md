

# libflow日志获取指导
## 说明
- 我司提供的websocket服务一般是基于自研的libflow服务模块提供的。
- 因此本日志获取指导只适合我司提供程序的websocket接口的调试。

## 指导
1. 设置环境变量，打开libflow的日志，然后再启动使用了libflow的Server/Client程序。
```
# 举例: run_dms.sh脚本启动了一个使用了libflow的Server，
# 则可以使用下述命令打开websocket相关的日志，然后再启动Sever。
export LIBFLOW_LOG_LEVEL=Verbose
./run_dms.sh
```
```
# 打印出来的日志开头为 [flow:
#     一般有：[flow:Debug]、[flow:Error] 等
# 下面就是一些真实打印出来的日志：
[flow:Debug] OnRecvOutputMessage: received an outmsg with topic: seat_cabin_1, queue length: 1
[flow:Debug] OnRecvOutputMessage: dropped an outmsg with topic seat_cabin_1
[flow:Debug] there are no clients for topic seat_cabin_1
```

2. 通过关键字搜索/过滤关键的日志
- 查看Server的连接建立的日志（也即Client的连接到Server的日志）：
  - 关键字：`accepted connection`
  - 真实日志举例：`[flow:Debug] accepted connection 0x7f4848000e90`
- 查看Server的断开连接的日志（也即Client断开与Server连接的日志）：
  - 关键字：`has closed`
  - 真实日志举例：`[flow:Debug] the 0-th client 0x7f4848000e90 (0x7f4848000f98) has closed`
- 查看当前Client的数量：
  - 关键字： `current client count`
  - 真实日志举例：`[flow:Debug] current client count: 1`
- 查看某一topic的通讯日志
  - 关键字：topic的名称，如：`seat_cabin_1`
  - 真实日志举例：
    - `[flow:Debug] OnRecvOutputMessage: received an outmsg with topic: seat_cabin_1, queue length: 1`
    - `[flow:Debug] OnRecvOutputMessage: dropped an outmsg with topic seat_cabin_1`
    - ``
- 查看Server发送的消息的日志：
  - 关键字：`OnRecvOutputMessage`
  - 真实日志举例：
    - `[flow:Debug] OnRecvOutputMessage: received an outmsg with topic: seat_cabin_1, queue length: 1`
    - `[flow:Debug] send_mbuf of 0x7f4848000e90: len=366, size=549`
    - `[flow:Debug] the 0-th client 0x7f4848000e90 (0x7f4848000f98) has just send 366 bytes, send_mbuf.len: 0`

- 查看Server向某一个Client发送的消息的日志：
  - 关键字：Client的id & `has just send`
  - 真实日志举例：`[flow:Debug] the 0-th client 0x7f4848000e90 (0x7f4848000f98) has just send 365 bytes, send_mbuf.len: 0`

- 查看Client subscribed 的topic的情况：
  - 关键字： `subscribed`
  - 真实日志举例：`[flow:Debug] connection 0x7f4848000e90 (0x7f4848000f98) subscribed to *`

3. 还有更多的关键字，你可以参考我们提供的真实日志文件`libflow_debug.log`自行按需设定。

