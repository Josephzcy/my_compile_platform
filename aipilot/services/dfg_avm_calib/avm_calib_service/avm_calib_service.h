//
// Created by minieye on 2022/6/8.
//

#ifndef AIPLORER_AVMCALIBSERVICE_H
#define AIPLORER_AVMCALIBSERVICE_H

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <vector>

#include "aipilot/services/dfg_avm_calib/avm_calib_service/WrapperDiag.h"
#include "aipilot/services/dfg_avm_calib/src/avm_calib.h"
#include "msg/precompiler/avm_calib_ctrl.pb.h"
#include "msg/precompiler/avm_calib_param.pb.h"
#include "msg/precompiler/camera.pb.h"
#include "dds_api.h"
#include "dds_common.h"
#include "opencv2/opencv.hpp"

namespace minieye {
namespace avm {
class AvmCalibService {
public:
  bool init(const std::string &config_file,
            std::map<std::string, std::string> &topic_names,
            std::map<int32_t, std::string> &intrins_files,
            std::string &car_info, std::string &layout_file,
            std::string &savedPath, float reproject_err = 0.02f);

  void uninit();

  static void run(AvmCalibService *ptr);

  void save_camera(minieye::CameraFrame camera);

  void setCalibCtrl();

  void resetAvmCalibStatus();

private:
  void getCalibCtrl();

  /*
  @brief: read data, store to vector.
  @param camera[out]: camera data from dds
  @param camera_time_ms[out]: current camera received timestamp.
  */
  void read_data(cv::Mat &img, uint64_t &camera_time_ms);

  int getInputImage(const minieye::CameraFrame *camera, cv::Mat &bgr);

  void changeAttribut();

  void setDiagCode();

  /**
   * 获取车辆标定状态并通过诊断模块进行发送与恢复
   */
  void getVehicleCalibStatus(const std::string &status_file);

  void writeCalibStatus(const std::string &status_file);

public:
  std::mutex m_lock;
  std::mutex m_lock_ctrl;
  std::condition_variable m_cond_camera;
  std::condition_variable m_cond_ctrl;
  std::string m_dds_topic_camera;
  std::string m_dds_topic_ctrl;
  std::string m_savedPath;
  std::atomic<bool> m_beginToCalib;

private:
  bool m_init_state;
  float m_fReprojectErr;
  uint64_t m_camera_time_ms = 0; // 收到 m_camera 的时间戳

  std::vector<std::string> m_intrins_files;
  std::string m_statusFile;
  AvmHandle m_avmCalibHandle;
  AvmCalibState m_avmState;
  minieye::CameraFrame m_camera;
  minieye::DDS::Context *m_ctx;
  minieye::DDS::Writer *m_avmStatusWriter;

  std::map<int, std::tuple<int, int, int>> m_camera_calib_err;
  std::map<int, std::tuple<int, int, int>> m_camera_calib_conner;
  std::map<int, std::tuple<int, int, int>> m_camera_calib_saved;
};
} // namespace avm
} // namespace minieye
#endif // AIPLORER_AVMCALIBSERVICE_H
