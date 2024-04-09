#ifndef _DDS_CONNECTION_MANAGER_H_
#define _DDS_CONNECTION_MANAGER_H_

#include "connection_manager.hpp"
#include "dds_api.h"
#include "dds_common.h"

namespace module {
namespace connection {
template <typename T, typename DDS_MANAGER_TYPE>
void Callback(const char *topic, size_t index, void *ptr, size_t size, void *user) {
    DDS_MANAGER_TYPE *dds_connection_manager = static_cast<DDS_MANAGER_TYPE *>(user);
    if (dds_connection_manager->Suspend()) {
        dds_connection_manager->ClearData(topic);
        return;
    }
    char *str = static_cast<char *>(ptr);
    std::string data_str(str, size);
    dds_connection_manager->InsertData(topic, data_str);
}

namespace {
template <typename Tuple, typename DDSManagerType, size_t N>
struct DdsManagerTupleItor {
    static void MakeSubBuffer(int &idx, const std::vector<std::string> &sub_topics,
                              std::deque<std::shared_ptr<minieye::DDS::Reader>> &reader_deque,
                              minieye::DDS::Context *ctx, void *manager_ptr) {
        DdsManagerTupleItor<Tuple, DDSManagerType, N - 1>::MakeSubBuffer(idx, sub_topics, reader_deque, ctx,
                                                                         manager_ptr);
        const std::string topic_name = sub_topics[idx];
        using ElementType = typename std::tuple_element<N - 1, Tuple>::type;
        reader_deque.emplace_back(std::make_shared<minieye::DDS::Reader>(
            ctx, topic_name, Callback<ElementType, DDSManagerType>, manager_ptr));
        ++idx;
    }
};

template <typename Tuple, typename DDSManagerType>
struct DdsManagerTupleItor<Tuple, DDSManagerType, 0> {
    static void MakeSubBuffer(int &idx, const std::vector<std::string> &sub_topics,
                              std::deque<std::shared_ptr<minieye::DDS::Reader>> &reader_deque,
                              minieye::DDS::Context *ctx, void *manager_ptr) {}
};
}  // namespace

template <typename... StordTypes>
class DdsConnectionManager : public ConnectionManagerBase<StordTypes...> {
   public:
    using ParentClass = ConnectionManagerBase<StordTypes...>;

    DdsConnectionManager(const ConnectionManagerConfig &connection_manager_config)
        : ParentClass(connection_manager_config) {}
    virtual ~DdsConnectionManager() override { delete ctx_; }

    virtual void Init() override {
        std::string json_file = ParentClass::GetConfig().other_params[0];
        ctx_ = new minieye::DDS::Context(json_file, true);

        using StordTuple = std::tuple<StordTypes...>;
        int idx = 0;
        DdsManagerTupleItor<StordTuple, DdsConnectionManager<StordTypes...>, std::tuple_size<StordTuple>::value>::
            MakeSubBuffer(idx, ParentClass::GetConfig().subscribe_topics, reader_deque_, ctx_, this);

        for (size_t i = 0; i < ParentClass::GetConfig().publish_topics.size(); ++i) {
            const std::string &topic_name = ParentClass::GetConfig().publish_topics[i];
            pub_idx_map_[topic_name] = publish_vec_.size();
            publish_vec_.emplace_back(std::make_shared<minieye::DDS::Writer>(ctx_, topic_name));
        }
    }

    virtual void PublishData(const std::string &data_name, const void *buf, uint32_t buf_length) override {
        if (!pub_idx_map_.count(data_name)) {
            LOG(ERROR) << "no " << data_name << " found, could not write";
            return;
        }
        if (buf_length == 0) {
            LOG(ERROR) << "no buffer len";
            return;
        }
        publish_vec_[pub_idx_map_[data_name]]->Write(buf, buf_length);
    }

   private:
    minieye::DDS::Context *ctx_;
    std::deque<std::shared_ptr<minieye::DDS::Reader>> reader_deque_;

    std::map<std::string, int> pub_idx_map_;
    std::deque<std::shared_ptr<minieye::DDS::Writer>> publish_vec_;
};
}  // namespace connection
}  // namespace module
#endif