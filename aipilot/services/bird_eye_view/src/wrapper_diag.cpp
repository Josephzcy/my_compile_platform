// Copyright [2022] - MiniEye INC.
#include "wrapper_diag.h"

#include <thread>

#include "glog/logging.h"
#include "me_fm_client.h"
#include "me_hm_client.h"

CWrapperDiag CWrapperDiag::instance_;

CWrapperDiag &CWrapperDiag::instance() { return instance_; }

CWrapperDiag::CWrapperDiag() {
  // 初始化看门狗
  minieye::hm::MeHMClient::Instance()->Init("bird_eye_view", nullptr);
  minieye::hm::MeHMClient::Instance()->RegistWatchdog();
  auto feeddog_thread = []() {
    while (true) {
      minieye::hm::MeHMClient::Instance()->FeedWatchdog();
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  };
  std::thread(feeddog_thread).detach();

  // 上报故障
  minieye::FM::FMAgent::Instance()->Init(ALGO_MODULE_APA_BEV);
  // LOG(INFO) << "Init CWrapperDiag done.";

  // 初始时重置上次故障
  CWrapperDiag::instance().ErrorRecover(CAPA_PROCEDURE, FAPA_OPENGL_ERROR, EBEVOPENVXERROR);
  CWrapperDiag::instance().ErrorRecover(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVICAMNOTDATA);
  CWrapperDiag::instance().ErrorRecover(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVODDSERROR);
}

CWrapperDiag::~CWrapperDiag() {}

int CWrapperDiag::ReportError(uint32_t component, uint32_t function, uint32_t error_code) {
  uint32_t fault_id = minieye::FM::FMAgent::Instance()->GenFaultID(component, function, error_code);
  bool ret = minieye::FM::FMAgent::Instance()->ReportFault(fault_id, 1);
  if (!ret) {
    LOG(WARNING) << "report error failed, component = " << component << ", function = " << function
                 << ", error_code = " << error_code;
  }
  return 0;
}

int CWrapperDiag::ErrorRecover(uint32_t component, uint32_t function, uint32_t error_code) {
  uint32_t fault_id = minieye::FM::FMAgent::Instance()->GenFaultID(component, function, error_code);
  bool ret = minieye::FM::FMAgent::Instance()->ReportFault(fault_id, 0);
  if (!ret) {
    LOG(INFO) << "error recover send failed, component = " << component << ", function = " << function
              << ", error_code = " << error_code;
  }
  return 0;
}
