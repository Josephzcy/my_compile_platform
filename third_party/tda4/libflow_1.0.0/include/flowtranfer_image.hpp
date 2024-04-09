#ifndef __IMAGE_FLOW_H__
#define __IMAGE_FLOW_H__
#include "flow.hpp"
#include <vector>
#include <functional>
#include <cstring>
#include <mutex>
#include <iostream>

/* ex:
 * FlowSend *ptr = new FlowSend("0.0.0.0","24014");
 * ptr->send("camera0",...)
 *
 * */

class FlowSend {
   public:
    FlowSend(const char* addr, const char* port) {
        int ret = 0;
        FLowContext = flow::Context({
            {"addr", addr},
            {"port", port},
            {"sender.serialize", "raw"},
            {"queue.image.capacity", "16"},
        });

        FLowSender = new flow::Sender(&FLowContext, "Sender");

        ret = FLowContext.start();
        if (ret != 0) {
            abort();
        }
    }
    ~FlowSend() {
        FLowContext.stop();
        if (FLowSender) {
            delete FLowSender;
            FLowSender = nullptr;
        }
    }

    int send(char* topic, char* data, int size) {
        FLowSender->send(topic, data, size);
        return 0;
    }
    int send(char* topic, char* data, int size, char* head, int head_size) {
        unsigned int tmp_size = size + head_size;
        if (tmp_size > m_sendsize) {
            m_sendsize = tmp_size;

            m_buffer.reserve(m_sendsize);
            m_buffer.resize(m_sendsize, 0);
        }
        std::memcpy((void*)&m_buffer[0], (void*)head, head_size);
        std::memcpy((void*)(&(m_buffer[0]) + head_size), (void*)data, size);

        return send(topic, (char*)&m_buffer[0], size + head_size);
    }

   private:
    flow::Sender* FLowSender;
    flow::Context FLowContext;
    std::vector<unsigned char> m_buffer;
    unsigned int m_sendsize = 0;
};

/* ex:
 * void recv(const char *topic,const char *data,size_t size)
 * {
 *     std::cout << size << std::endl;
 * }
 * FlowRecv *ptr = new FlowRecv("0.0.0.0","24014","camera0",&recv);
 *
 *
 * */
class FlowRecv : public flow::Client {
   public:
    FlowRecv(const char* addr, const char* port, const char* topic,
             std::function<void(const char*, const char*, size_t)> callback)
        : flow::Client(
              flow::ClientConfig({"image",
                                  std::string("ws://") + std::string(addr) +
                                      std::string(":") + std::string(port),
                                  std::string(topic)})),
          FLowContext(flow::Config(
              {{"servers", "none"}, {"sender.serialize", "raw"}})),
          callback_(std::move(callback)) {
        FLowContext.add_client(this);
        if (FLowContext.start() != 0) {
            fprintf(stderr, "Error: cannot start flow context\n");
            exit(1);
        }
        fprintf(stderr, "  start flow context\n");
    }

    ~FlowRecv() { FLowContext.stop(); }

    virtual void recv(const char* source,  // '\0' terminated string
                      const char* topic,   // any binary data
                      const char* data,    // any binary data
                      size_t size) {
        std::lock_guard<std::mutex> lock(mutex_);
        this->callback_(topic, data, size);
    }

   private:
    flow::Context FLowContext;
    std::mutex mutex_;
    std::function<void(const char*, const char*, size_t)> callback_;
};
#endif
