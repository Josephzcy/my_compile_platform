//
// Created by mini on 2022/7/14.
//

#include "aipilot/services/dfg_avm_calib/avm_calib_service/WrapperDiag.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "diag_code.h"
#include "diag_manager.h"
#include "glog/logging.h"
#include "me_em_client.h"
#include "me_fm_client.h"
#include "me_hm_client.h"
#include "me_rm_client.h"
#include "aipilot/services/dfg_avm_calib/include/version.h"

using namespace minieye;

static std::atomic<bool> g_is_camera_front_valid;
static std::atomic<bool> g_is_camera_rear_valid;
static std::atomic<bool> g_is_camera_left_valid;
static std::atomic<bool> g_is_camera_right_valid;

CWrapperDiag& CWrapperDiag::instance() {
  static CWrapperDiag i;
  return i;
}

// 事件接收
void EventRcvCallback(const EM::EventData* eventData) {
  switch (eventData->eventType) {
    // avm 前摄像头故障
    case EM::EventType::EVENT_TYPE_AVMFRONTCAMFAULT:
      if (eventData->eventStatus == 1) {
        LOG(WARNING) << ("Recving camera front fault");
        g_is_camera_front_valid.store(false);
      } else {
        LOG(WARNING) << ("Recving camera front recover");
        g_is_camera_front_valid.store(true);
      }
      break;
      // avm 后摄像头故障
    case EM::EventType::EVENT_TYPE_AVMREARCAMFAULT:
      if (eventData->eventStatus == 1) {
        LOG(WARNING) << ("Recving camera rear fault");
        g_is_camera_rear_valid.store(false);
      } else {
        LOG(WARNING) << ("Recving camera rear recover");
        g_is_camera_rear_valid.store(true);
      }
      break;
      // avm 左摄像头故障
    case EM::EventType::EVENT_TYPE_AVMLEFTCAMFAULT:
      if (eventData->eventStatus == 1) {
        LOG(WARNING) << ("Recving camera left fault");
        g_is_camera_left_valid.store(false);
      } else {
        LOG(WARNING) << ("Recving camera left recover");
        g_is_camera_left_valid.store(true);
      }
      break;
      // avm 右摄像头故障
    case EM::EventType::EVENT_TYPE_AVMRIGHTCAMFAULT:
      if (eventData->eventStatus == 1) {
        LOG(WARNING) << ("Recving camera right fault");
        g_is_camera_right_valid.store(false);
      } else {
        LOG(WARNING) << ("Recving camera right recover");
        g_is_camera_right_valid.store(true);
      }
      break;
    default:
      std::cout << "rcv event default:" << eventData->eventType << std::endl;
      break;
  }
}

// 接收与回复请求
void RecvRequestCallback(const RM::ModuleCommand Command) {
  switch (Command) {
    case RM::MODULE_COMMAND_VERISON: {
      std::string version = std::string("AVM-CALIB-SERVER_T1Q_V") +
                            PROJECT_VER + "_" + BUILD_TIME_2;
      RM::RMAgent::Instance()->Response(version);
      break;
    }
    default:
      break;
  }
}

CWrapperDiag::CWrapperDiag() {
  g_is_camera_front_valid.store(true);
  g_is_camera_rear_valid.store(true);
  g_is_camera_left_valid.store(true);
  g_is_camera_right_valid.store(true);

  // 初始化事件消息
  EM::EventAgent::Instance()->Init();
  EM::EventAgent::Instance()->SetOnReceivingEventCallBack(EventRcvCallback);

  // 初始化请求回复
  bool init_ok = RM::RMAgent::Instance()->Init(RM::PROCESS_AVM_CALIB_SERVER);
  if (init_ok) {
    RM::RMAgent::Instance()->SetOnRequestCallBack(RecvRequestCallback);
  } else {
    LOG(ERROR) << ("RM::RMAgent init failed");
  }

  // 初始化看门狗
  hm::MeHMClient::Instance()->Init("avm_calib_service", nullptr);
  hm::MeHMClient::Instance()->RegistWatchdog();
  auto feeddog_thread = []() {
    while (true) {
      hm::MeHMClient::Instance()->FeedWatchdog();
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  };
  std::thread(feeddog_thread).detach();

  // 上报故障
  FM::FMAgent::Instance()->Init(ALGO_MODULE_APA_AVM);
}

CWrapperDiag::~CWrapperDiag() {}

bool CWrapperDiag::is_camera_front_valid() { return g_is_camera_front_valid; }

bool CWrapperDiag::is_camera_rear_valid() { return g_is_camera_rear_valid; }

bool CWrapperDiag::is_camera_left_valid() { return g_is_camera_left_valid; }

bool CWrapperDiag::is_camera_right_valid() { return g_is_camera_right_valid; }

int CWrapperDiag::report_error(uint32_t component, uint32_t function,
                               uint32_t error_code) {
  uint32_t fault_id =
      FM::FMAgent::Instance()->GenFaultID(component, function, error_code);
  bool ret = FM::FMAgent::Instance()->ReportFault(fault_id, 1);
  if (!ret) {
    LOG(ERROR) << "report error failed, component = " << component
               << ", function = " << function
               << ", error_code = " << error_code;
  }
  return 0;
}

int CWrapperDiag::error_recover(uint32_t component, uint32_t function,
                                uint32_t error_code) {
  uint32_t fault_id =
      FM::FMAgent::Instance()->GenFaultID(component, function, error_code);
  bool ret = FM::FMAgent::Instance()->ReportFault(fault_id, 0);
  if (!ret) {
    LOG(ERROR) << "error recover send failed, component = " << component
               << ", function = " << function
               << ", error_code = " << error_code;
  }
  return 0;
}
