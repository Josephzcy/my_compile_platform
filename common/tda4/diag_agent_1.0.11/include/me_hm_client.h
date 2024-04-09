
#ifndef ME_HM_CLIENT_H
#define ME_HM_CLIENT_H

#include <mutex>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <stdint.h>
#include <thread>
#include <dlt/dlt.h>
#include "nn_reqrep.h"

namespace minieye {
namespace hm {

#define HM_TYPE_HEALTH_CHECK    0x0
#define HM_TYPE_WATCH_DOG          0x1

struct AliveDesc {
    uint32_t type;
    uint32_t checkpointId;
    uint32_t periodMs;
    uint32_t expectedIndication;
    uint32_t minMargin;
    uint32_t maxMargin;
    uint32_t aliveIndicate;
    uint32_t checkTime;
};

class MeHMClient final {
public:
    static MeHMClient *Instance();

    /**  MeHMClient 初始化
     @param[in]  processName 进程名
     @param[in]  callback 初始化结果的回调
     @param[out] none
     @return     void
     @warning    初始化成功后，才能调用其他功能接口
     @note       无
    */
    void Init(const std::string processName, std::function<void(bool)> callback);

    /**  MeHMClient 反初始化
     @param[in]  none
     @param[out] none
     @return     void
     @warning    无
     @note       无
    */
    void DeInit();

    /** 注册心跳看门狗
    @param[in]  none
     @param[out] none
     @return     int 心跳看门狗注册结果，0注册成功，非0注册失败
     @warning    无
     @note       只要注册就需要定期喂狗，否则会向诊断服务上报故障
    */
   int RegistWatchdog(void);

    /** 反注册心跳看门狗
    @param[in]  none
     @param[out] none
     @return     none
     @warning    无
     @note
    */
    void UnRegistWatchdog(void);

     /** 喂心跳看门狗
    @param[in]  none
     @param[out] none
     @return     none
     @warning    无
     @note
    */
    void FeedWatchdog(void);

    /**  注册Alive监控任务
     @param[in]  checkpointId 检查点
     @param[in]  periodMs 监控周期
     @param[in]  expectedIndication 监控周期内上报检查点的次数
     @param[in]  minMargin 向下容忍度
     @param[in]  maxMargin 向上容忍度
     @param[out] none
     @return     bool 注册结果
     @warning    调用此接口前，需完成Init调用，并确保服务是可用的;
                 checkpointID为0设计的是进程保活用的周期任务，默认每个进程都会分配该ID
     @note       注册Alive监控任务，可用于进程定期上报心跳或其它周期处理任务
    */
    bool RegistAliveTask(const std::uint32_t checkpointId, const std::uint32_t periodMs,
                         const std::uint32_t expectedIndication, const std::uint32_t minMargin, const std::uint32_t maxMargin);
    
    /**  反注册Alive监控任务
     @param[in]  checkpointId 注册时传入的checkpointId
     @param[out] none
     @return     bool 反注册结果
     @warning    调用此接口前，需成功调用RegistAliveTask()接口
     @note       反注册Alive监控任务，用于主动释放资源
    */
    bool UnRegistAliveTask(const std::uint32_t checkpointId);

    /**  注册Deadline监控任务
     @param[in]  srcCheckpointId 上报的源检查点
     @param[in]  destCheckpointId 上报的目的检查点
     @param[in]  deadlineMinMs 最小截止时间
     @param[in]  deadlineMaxMs 最大截止时间
     @param[out] none
     @return     bool 注册结果
     @warning    调用此接口前，需完成Init调用，并确保服务是可用的
     @note       注册Deadline监控任务，可用监控检查两个检查点之间的增量时间
    */
    bool RegistDeadlineTask(const std::uint32_t srcCheckpointId, const std::uint32_t destCheckpointId,
                            const std::uint32_t deadlineMinMs, const std::uint32_t deadlineMaxMs);

    /**  反注册Deadline监控任务
     @param[in]  srcCheckpointId 上报的源检查点
     @param[in]  destCheckpointId 上报的目的检查点
     @param[out] none
     @return     bool 反注册结果
     @warning    RegistDeadlineTask()接口
     @note       反注册Deadline监控任务，用于主动释放资源
    */
    bool UnRegistDeadlineTask(const std::uint32_t srcCheckpointId, const std::uint32_t destCheckpointId);

    /**  注册Logical监控任务
     @param[in]  checkpointIds 上报的检查点序列
     @param[out] none
     @return     bool 注册结果
     @warning    调用此接口前，需完成Init调用，并确保服务是可用的
     @note       注册Logic监控任务，可用监控检查点执行的正确顺序
    */
    bool RegistLogicTask(std::vector<std::uint32_t> checkpointIds);

    /**  反注册Logical监控任务
     @param[in]  checkpointIds 上报的检查点序列
     @param[out] none
     @return     bool 反注册结果
     @warning    RegistLogicTask()接口
     @note       反注册Logical监控任务，用于主动释放资源
    */
    bool UnRegistLogicTask(std::vector<std::uint32_t> checkpointIds);

    /**  上报检查点
     @param[in]  checkpointId 上报的检查点（注册成功的检查点）
     @param[out] none
     @return     bool 上报结果
     @warning    调用此接口前，需完成Init调用，并确保服务是可用的
     @note       注册了三种监控任务后，根据实际情况，在需要上报检查点的时候，调用此接口
    */
    void ReportCheckpoint(const std::uint32_t checkpointId);

private:
    //! 构造函数
    MeHMClient();
    //! 析构函数
    ~MeHMClient();

    //! Alive任务超期回调
    void AliveTimeoutHook(std::uint32_t checkpointId);

    //! Alive任务OverRun回调
    void AliveOverRunHook(std::uint32_t checkpointId);

    //! watchdog超期回调
    void WatchdogPeriodHook(std::uint32_t  tick);

    static MeHMClient *instancePtr_;

    std::string processName_;
    pthread_mutex_t mtx_;
     pthread_cond_t cond_;
    std::shared_ptr<nn_req> req_;
    std::shared_ptr<std::thread> th_;
    bool runing_;
    std::vector<AliveDesc> aliveTaskMap_;
    DltContext context;
};

}  // namespace hm
}  // namespace minieye
#endif  // ME_HM_CLIENT_H
