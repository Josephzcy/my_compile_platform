#pragma once
#include <functional>
#include <memory>
#include "diag_manager.h"

namespace minieye {
namespace EM {

class EventAgent {
   public:
    static EventAgent* Instance();
    bool Init();
    void SetOnReceivingEventCallBack(std::function<void(const EM::EventData*)> callback);
    
   private:
    class EventAgentImpl;
    std::shared_ptr<EventAgentImpl> ea_impl_;
   
    ~EventAgent() = default;
    EventAgent() = default;
    EventAgent& operator=(const EventAgent&) = delete;
    EventAgent(const EventAgent&) = delete;

};

};  // namespace EM
}; // namespace minieye
