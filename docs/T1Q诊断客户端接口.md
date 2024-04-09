# T1Q诊断客户端接口

## 概述

接口库提供客户端的诊断接口，有以下四种接口

###### 1.故障上报接口

应用产生故障后，将对应故障的id和对象通过接口上报，服务端统一收集处理故障。

###### 2.广播事件的接收

服务端将状态机切换,系统消息发布等事件广播发布，客户端接收事件做相应处理。

###### 3.服务端请求的接收和回复

服务端向客户端请求反馈消息,比如版本状态等，客户端接收并向服务端反馈。

###### 4.健康管理心跳保持

健康管理接口提供Alive监控功能。Alive监控检查点是否以正确的频率发生，通常用在检查周期函数是否正常的执行；监控功能通过一个代理提供对外使用的API和动态链接库，代理将提供监控的注册接口及检查点上报接口。监控代理将与监控服务端通信，通过一定的交互完成监控功能，并在监控服务端对监控的结果映射为相应的故障，上报给故障管理模块。

监控需求由上层应用主动提出，健康管理模块开发人员分配检查点，上层应用根据实际情况上报分配的检查点，应用程序默认可以使用checkpointId为0的Alive监控检查的默认检查点。

## 目录结构

```
├── include  
│   ├── diag_code.h				 #故障码头文件
│   ├── diag_manager.h				 #诊断管理用到的结构体、枚举
│   ├── me_em_client.h				 #事件接收类头文件
│   ├── me_fm_client.h				 #故障上报类头文件
│   ├── me_hm_client.h				 #健康管理心跳类头文件
│   ├── me_rm_client.h				 #请求回复类头文件
│   └── nn_reqrep.h				 #辅助类头文件
├── lib
│   ├── libDiagAgent.so				 #诊断客户端四个类的实现
│   ├── libgtctimer.so				 #诊断接口的依赖库
│   └── libnanomsg.so				 #诊断接口的依赖库
├── T1Q健康管理接口说明.docx
└── t1q诊断管理接口说明.md
```

## 接口定义说明



健康管理类说明见 T1Q健康管理接口说明.docx。

故障上报类 FMAgent：

|    类    |                 故障上报类                  |
| :------: | :-----------------------------------------: |
| 函数原型 |         static FMAgent* Instance();         |
| 函数功能 |      单例模式,获得唯一的 FMAgent 实例       |
|   参数   |                    void                     |
|  返回值  |              FMAgent 实例指针               |
|  头文件  |               me_fm_client.h                |
| 使用样例 | auto fualt_agent = FM::FMAgent::Instance(); |
| 注意事项 |                  单例模式                   |

​								

|    类    |                       故障上报类                        |
| :------: | :-----------------------------------------------------: |
| 函数原型 |              bool Init(uint32_t algo_id);               |
| 函数功能 |              初始化FMAgent并绑定算法模块ID              |
|   参数   |              uint32_t algo_id :模块 ID 号               |
|  返回值  |            bool , true 为成功, false 为失败             |
|  头文件  |                     me_fm_client.h                      |
| 使用样例 | FM::FMAgent::Instance()->Init(ALGO_MODULE_CAMERA_LANE); |
| 注意事项 |                   每个模块填自己的 ID                   |



|    类    |                          故障上报类                          |
| :------: | :----------------------------------------------------------: |
| 函数原型 | bool ReportFault(uint32_t component, uint32_t function, uint32_t error_code); |
| 函数功能 |                          上报故障ID                          |
|   参数   | component：故障发生组件， function：故障发生功能  ，error_code：故障码 |
|  返回值  |               bool , true 为成功, false 为失败               |
|  头文件  |                        me_fm_client.h                        |
| 使用样例 | FM::FMAgent::Instance()->ReportFault(CAMERA_LANE_INPUT, Camera_Lane_CNN_ACC, EPERM); |
| 注意事项 |        故障参数在diag_code.h,如没有找到，需要讨论增加        |



|    类    |                  事件接收类                   |
| :------: | :-------------------------------------------: |
| 函数原型 |        static EventAgent* Instance();         |
| 函数功能 |      单例模式,获得唯一的 EventAgent 实例      |
|   参数   |                     void                      |
|  返回值  |              EventAgent 实例指针              |
|  头文件  |                me_em_client.h                 |
| 使用样例 | auto event_agent = EM::EventAgent::Instance() |
| 注意事项 |                   单例模式                    |



|    类    |             事件接收类              |
| :------: | :---------------------------------: |
| 函数原型 |            bool Init();             |
| 函数功能 |          初始化EventAgent           |
|   参数   |                void                 |
|  返回值  |  bool , true 为成功, false 为失败   |
|  头文件  |           me_em_client.h            |
| 使用样例 | EM::EventAgent::Instance()->Init(); |
| 注意事项 |     配置文件里url要与Server一致     |



|    类    |                          事件接收类                          |
| :------: | :----------------------------------------------------------: |
| 函数原型 | void SetOnReceivingEventCallBack(std::function <void(EM::EventType)> callback); |
| 函数功能 |                    设置接收事件的回调函数                    |
|   参数   | std::function <void(EM::EventType)> callback ：回调函数，接收事件 |
|  返回值  |                             void                             |
|  头文件  |                        me_em_client.h                        |
| 使用样例 | EM::EventAgent::Instance()->SetOnReceivingEventCallBack(EventRcvCallback); |
| 注意事项 |          所有事件都会触发回调，需要根据实际情况过滤          |



|    类    |                  请求回复类                  |
| :------: | :------------------------------------------: |
| 函数原型 |         static RMAgent* Instance();          |
| 函数功能 |       单例模式,获得唯一的 RMAgent 实例       |
|   参数   |                     void                     |
|  返回值  |               RMAgent 实例指针               |
|  头文件  |                me_rm_client.h                |
| 使用样例 | auto request_agent = RM::RMAgent::Instance() |
| 注意事项 |                   单例模式                   |



|    类    |                    请求回复类                     |
| :------: | :-----------------------------------------------: |
| 函数原型 |           bool Init(RM::ProcessID id);            |
| 函数功能 |           初始化 RMAgent 并绑定模块 ID            |
|   参数   |              ModuleID id :模块 ID 号              |
|  返回值  |         bool , true 为成功, false 为失败          |
|  头文件  |                  me_rm_client.h                   |
| 使用样例 | RM::RMAgent::Instance()->Init(RM::MODULE_FUSION); |
| 注意事项 |  每个模块填自己的 ID,配置文件里url要与Server一致  |



|    类    |                          请求回复类                          |
| :------: | :----------------------------------------------------------: |
| 函数原型 | bool SetOnRequestCallBack(std::function<void(const RM::ModuleCommand &cmd)> callback); |
| 函数功能 |                    设置接收请求的回调函数                    |
|   参数   | std::function<void(const RM::ModuleCommand &cmd)> callback ：回调函数，接收请求 |
|  返回值  |               bool , true 为成功, false 为失败               |
|  头文件  |                        me_rm_client.h                        |
| 使用样例 | RM::RMAgent::Instance()->SetOnRequestCallBack(RecvRequestCallback); |
| 注意事项 |              回调中处理命令，不能做太耗时的操作              |



|    类    |                   请求回复类                   |
| :------: | :--------------------------------------------: |
| 函数原型 |  bool Response(RM::ResponseData replayData);   |
| 函数功能 |                    回复请求                    |
|   参数   | RM::ResponseData replayData ：回复信息的结构体 |
|  返回值  |        bool , true 为成功, false 为失败        |
|  头文件  |                 me_rm_client.h                 |
| 使用样例 | RM::RMAgent::Instance()->Response(replayData); |
| 注意事项 |         回复不能和接收请求时间间隔太久         |



## 主要结构体和枚举



EM:事件类型枚举，客户端根据不同事件类型，做对应响应。后续增加其它事件类型。

```c++
typedef enum EventType {
    EVENT_TYPE_MIN = 0,
    EVENT_TYPE_ENOCHASISDATA,
    EVENT_TYPE_ENOCANSPEED,
    EVENT_TYPE_ECAMERANOTCALIBRATED,
    EVENT_TYPE_EFORWARDFAILURE,
    EVENT_TYPE_ESENDFAILED,
    EVENT_TYPE_ECAMERADATALOST1,
    EVENT_TYPE_ECAMERADATALOST2,
    EVENT_TYPE_ECAMERADATALOST3,
    EVENT_TYPE_ECAMERADATALOST4,
    EVENT_TYPE_ECAMERADATALOST5,
    EVENT_TYPE_ECAMERADATALOST6,
    EVENT_TYPE_ECAMERADATALOST7,
    EVENT_TYPE_ECAMERADATALOST8,
    EVENT_TYPE_ECAMERADATALOST9,
    EVENT_TYPE_ECAMERACALIBRATEFAILED1,
    EVENT_TYPE_ECAMERACALIBRATEFAILED2,
    EVENT_TYPE_ECAMERACALIBRATEFAILED3,
    EVENT_TYPE_ECAMERACALIBRATEFAILED4,
    EVENT_TYPE_ECAMERACALIBRATEFAILED5,
    EVENT_TYPE_ECAMERACALIBRATEFAILED6,
    EVENT_TYPE_ECAMERACALIBRATEFAILED7,
    EVENT_TYPE_ECAMERACALIBRATEFAILED8,
    EVENT_TYPE_ECAMERACALIBRATEFAILED9,

    EVENT_TYPE_OBJECT_MAX = 30,

    EVENT_TYPE_ENOLONGFRONT,
    EVENT_TYPE_ENOREAR,
    EVENT_TYPE_ENOCHASIS,
    EVENT_TYPE_ESPEEDINVALID,
    EVENT_TYPE_ECALIBINVALID,
    EVENT_TYPE_EACCINITFAILED,
    EVENT_TYPE_EMODELLOADFAILED,

    EVENT_TYPE_LANE_MAX = 40,

    EVENT_TYPE_ENO1VMEA,
    EVENT_TYPE_ENOFRONTLANE,
    EVENT_TYPE_EVEHICLEPBPARSE,
    EVENT_TYPE_EFRONTLANEPBPARSE,
    EVENT_TYPE_EREARLANEPBPARSE,
    EVENT_TYPE_ENOFRONTRADAR,
    EVENT_TYPE_EFRONTRADARPBPARSE,
    EVENT_TYPE_ENOFRRADAR,
    EVENT_TYPE_ENOFLRADAR,
    EVENT_TYPE_ENORRRADAR,
    EVENT_TYPE_ENORLRADAR,
    EVENT_TYPE_ECRPBPARSE,
    EVENT_TYPE_ENOEGOCAR,
    EVENT_TYPE_EEGOPBPARSE,

    EVENT_TYPE_FUSION_MAX = 60,
    
    EVENT_TYPE_ENOFRONTLANE_PLAN,
    EVENT_TYPE_ENOFUSION,
    EVENT_TYPE_ENOVEHICLESINGAL,
    EVENT_TYPE_EFRONTLANEPBPARSE_PLAN,
    EVENT_TYPE_EFUSIONPBPARSE,
    EVENT_TYPE_EVEHICLESINGALPBPARSE, 
    EVENT_TYPE_ELIBFLOWSEND,
    EVENT_TYPE_EDDSSEND,

    EVENT_TYPE_PLANING_MAX = 70,

    EVENT_TYPE_SOCHARDWARE,

    EVENT_TYPE_MAX
}EventType_t;
```

RM:请求客户端回复的模块。

```c++
enum ProcessID {
    PROCESS_ID_MIN = 0,
    PROCESS_LANE,
    PROCESS_OBJECT_DETECT,
    PROCESS_FUSION,
    PROCESS_ALGO_MAX = 10,
    PROCESS_CANOUT,
    PROCESS_CHASSIS,
    PROCESS_RADAR,
    PROCESS_PRIVATE_CANOUT,
    PROCESS_CAN_MAX = 20,
    PROCESS_APP_FLEXIDAG_IMU,
    PROCESS_AVM_PARKING,
    PROCESS_APP_AVM_CAM_SERVER,
    PROCESS_APP_FRONT_CAM_SERVER,
    PROCESS_APP_MAX = 30,
    PROCESS_AVM_MANAGER,
    PROCESS_STATE_MANAGER,
    PROCESS_PHM_SERVER,
    PROCESS_FAULT_SERVER,
    PROCESS_SYS_SERVER,
    PROCESS_EVENT_SERVET,
    PROCESS_SERVER_MAX,
    PROCESS_ID_MAX = 40,
    PROCESS_ALL
};
```

RM:请求命令类型，客户端根据接收到的请求命令类型进行回复。目前只有版本号请求。后续增加其它命令类型。

```c++
enum ModuleCommand {
    MODULE_COMMAND_VERISON,
    MODULE_COMMAND_MAX
};
```

发送请求的结构体，包含需要相应请求的模块ID和命令。客户端根据命令类型回

```c++
struct ReqData {
    ProcessID id;
    ModuleCommand command;
};
```

相应请求的结构，包含回复的模块和版本号。后续丰富其它需要回复的内容

```c++
struct ResponseData {
    ProcessID id;
    char verison[64];  
};
```

## 使用示例

- 故障上报使用demo

```c++
#include <iostream>
#include <thread>
#include <chrono>
#include <iostream>
#include “me_fm_client.h”
#include “diag_code.h”

using namespace minieye;
int main(int argc, char* argv[])
{
    FM::FMAgent::Instance()->Init(ALGO_MODULE_CAMERA_LANE);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    /*在需要上报的地方调用,故障码由algo、component、function和errcode组成，
    具体模块名和错误码在diag_code.h*/
    FM::FMAgent::Instance()->ReportFault(CAMERA_LANE_INPUT, Camera_Lane_CNN_ACC, EPERM);
    
	while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
```

- 事件接收使用demo

```c++
#include <iostream>
#include <thread>
#include <chrono>
#include “me_em_client.h”
#include “diag_manager.h”
#include “diag_code.h”

void EventRcvCallback(EM::EventType eventType) {
    switch (eventType) {
        case EM::EventType::EVENT_TYPE_ENOCHASISDATA:
            std::cout << "rcv event:" << EM::EventType::EVENT_TYPE_ENOCHASISDATA
                      << std::endl;
            break;
        case EM::EventType::EVENT_TYPE_ESENDFAILED:
            std::cout << "rcv event: " << EM::EventType::EVENT_TYPE_ESENDFAILED
                      << std::endl;
            break;
        default:
            std::cout << "rcv event default:" << eventType
                      << std::endl;
            break;
    }
}

int main(int argc, char* argv[])
{
    EM::EventAgent::Instance()->Init();
    EM::EventAgent::Instance()->SetOnReceivingEventCallBack(EventRcvCallback);
    
	while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500U));
    }
    return 0;
}
```

- 接收与回复请求demo

```c++
#include <iostream>
#include <thread>
#include <chrono>
#include “diag_manager.h”
#include “me_rm_client.h”
#include "diag_code.h"

std::string Version = "EVE_T1Q_V.1.0.1_202205241712_01";
void RecvRequestCallback(const RM::ModuleCommand Command) {
    switch (Command) {
        case RM::MODULE_COMMAND_VERISON: {
            RM::ResponseData replayData;
            strcpy(replayData.verison, Version.c_str());
            RM::RMAgent::Instance()->Response(replayData);
            break;
        }
        default:
            break;
    }
}

int main(int argc, char* argv[])
{
    bool init_ok = RM::RMAgent::Instance()->Init( RM::PROCESS_CHASSIS);
    if (init_ok) {
        RM::RMAgent::Instance()->SetOnRequestCallBack(RecvRequestCallback);
    }
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500U));
    }
    return 0;
}
```

​		健康管理接口使用demo

```c++
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string.h>
#include <functional>
#include "me_hm_client.h"

using namespace std;
using namespace minieye::hm;

int main(int argc, char* argv[])
{
    /*初始化 HM Client*/
    MeHMClient::Instance()->Init("fusion", nullptr);
    MeHMClient::Instance()->RegistAliveTask(0, 100, 3, 1, 1);

    /*按照对应的频率调用接口*/
    while (true) {
        MeHMClient::Instance()->ReportCheckpoint(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    return 0;
}
```


 

