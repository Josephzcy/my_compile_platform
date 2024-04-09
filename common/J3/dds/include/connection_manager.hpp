#ifndef _CONNECTION_MANAGER_H
#define _CONNECTION_MANAGER_H

//#include <any>
#include <deque>
#include <map>
#include <memory>
#include <mutex>

#include "connection_manager_config.h"
#include "glog/logging.h"
namespace module::connection {

namespace {
template <typename Tuple, size_t N>
struct ConnectionManagerBaseTupleItor {
    static void MakeBuffer(const std::vector<std::string> sub_name, std::map<std::string, int>& sub_idx_map,
                           std::deque<std::deque<std::string>>& data_buffer) {
        ConnectionManagerBaseTupleItor<Tuple, N - 1>::MakeBuffer(sub_name, sub_idx_map, data_buffer);
        int idx = data_buffer.size();
        data_buffer.emplace_back(std::deque<std::string>());
        sub_idx_map[sub_name[idx]] = idx;
    }
};

template <typename Tuple>
struct ConnectionManagerBaseTupleItor<Tuple, 0> {
    static void MakeBuffer(const std::vector<std::string> sub_name, std::map<std::string, int>& sub_idx_map,
                           std::deque<std::deque<std::string>>& data_buffer) {}
};
}  // namespace
template <typename... StordTypes>
class ConnectionManagerBase {
   public:
    ConnectionManagerBase(const ConnectionManagerConfig& connection_manager_config)
        : connection_manager_config_(connection_manager_config), sub_idx_mutex_(sizeof...(StordTypes)), suspend_(false) {
        using StordTuple = std::tuple<StordTypes...>;
        LOG_IF(ERROR, sizeof...(StordTypes) != connection_manager_config_.subscribe_topics.size()) << "size of sub not correct!";
        ConnectionManagerBaseTupleItor<StordTuple, std::tuple_size<StordTuple>::value>::MakeBuffer(
            connection_manager_config_.subscribe_topics, sub_idx_map_, data_buffer_);
    }

    virtual ~ConnectionManagerBase() = default;

    // 重载，完成所有subscribers与publishers的初始化
    // 调用inset subscribers插入subscriber的指针
    virtual void Init() {}

    void InsertData(const std::string& data_name, std::string& data) {
        if (!sub_idx_map_.count(data_name)) {
            LOG(ERROR) << "no " << data_name << " found";
            return;
        }
        int idx = sub_idx_map_[data_name];
        std::lock_guard<std::mutex> lock(sub_idx_mutex_[idx]);
        data_buffer_[idx].emplace_back(std::move(data));
    }

    void ClearData(const std::string& data_name) {
        if (!sub_idx_map_.count(data_name)) {
            LOG(ERROR) << "no " << data_name << " found";
            return;
        }
        int idx = sub_idx_map_[data_name];
        std::lock_guard<std::mutex> lock(sub_idx_mutex_[idx]);
        data_buffer_[idx].clear();
    }

    template <typename T>
    void GetData(const std::string& data_name, std::deque<T>& res) {
        if (!sub_idx_map_.count(data_name)) {
            LOG(ERROR) << "no " << data_name << " included";
            return;
        }
        int idx = sub_idx_map_[data_name];
        // 分开锁
        sub_idx_mutex_[idx].lock();
        std::deque<std::string> res_str = std::move(data_buffer_[idx]);
        data_buffer_[idx] = std::deque<std::string>();
        sub_idx_mutex_[idx].unlock();
        for (const std::string& str : res_str) {
            T type;
            type.ParseFromString(str);
            res.emplace_back(std::move(type));
        }
    }

    // 只要还在用protobuffer，发出去的一定是这个形式
    // 那这里就必须有个write方法
    // 这个没法抽象出来
    virtual void PublishData(const std::string& data_name, const void* buf, uint32_t buf_length) = 0;

    const ConnectionManagerConfig& GetConfig() const { return connection_manager_config_; }

    bool Suspend() { return suspend_; }

    void SetSuspend(bool suspend) { suspend_ = suspend; }

   private:
    const ConnectionManagerConfig& connection_manager_config_;

    std::deque<std::mutex> sub_idx_mutex_;
    std::map<std::string, int> sub_idx_map_;
    // std::shared_ptr<std::deque<T>>
    std::deque<std::deque<std::string>> data_buffer_;

    bool suspend_;
};

}  // namespace module::connection

#endif
