#pragma once
#include <functional>
#include <memory>
#include "diag_manager.h"

namespace minieye {
namespace RM {

class RMAgent {
   public:
    static RMAgent* Instance();
    bool Init(RM::ProcessID id);
    bool SetOnRequestCallBack(std::function<void(const RM::ModuleCommand &cmd)> callback);
    bool Response(std::string &data);

   private:
    class RMAgentImpl;
    std::shared_ptr<RMAgentImpl> ra_impl_;

    ~RMAgent() = default;
    RMAgent() = default;
    RMAgent& operator=(const RMAgent&) = delete;
    RMAgent(const RMAgent&) = delete;

};

};  // namespace RM
};  //namespace minieye 
