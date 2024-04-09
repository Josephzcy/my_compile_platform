#pragma once
#include <functional>
#include <memory>
#include "diag_code.h"

namespace minieye {
namespace FM {

class FMAgent {
   public:
    static FMAgent* Instance();
    bool Init(uint32_t algo_id);
    uint32_t GenFaultID(uint32_t component, uint32_t function, uint32_t error_code);
    bool ReportFault(uint32_t fualtID, uint16_t faultStatus);

   private:
    class FaultAgentImpl;
    std::shared_ptr<FaultAgentImpl> fa_impl_;
    uint32_t algo_id_;
    
    FMAgent() = default;
    FMAgent& operator=(const FMAgent&) = delete;
    FMAgent(const FMAgent&) = delete;

};

};  // namespace FM
}; //minieye
