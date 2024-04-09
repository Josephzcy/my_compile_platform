//
// Created by minieye on 2022/6/8.
//
#include "avm_calib_service.h"

#include <sys/mount.h>

#include <condition_variable>
#include <mutex>

#include "TI/j7_tidl.h"
#include "TI/tivx.h"
#include "app_init.h"
#include "diag_code.h"
#include "diag_manager.h"
#include "glog/logging.h"
#include "json.hpp"

using namespace minieye::DDS;
using namespace minieye;

//#define AVM_CALIB_ERR (0.05f)

namespace minieye {
namespace avm {
// @param topic: topic
// @param index: 内部数组的下标，array[index]
// @param ptr: 是指向当前元素的指针
// @param size: 当前元素的大小（字节）
// @param user: 用户数据
void onData(const char *topic, size_t index, void *ptr, size_t size,
            void *user) {
  AvmCalibService *reader = (AvmCalibService *)user;
  if (strcmp(topic, reader->m_dds_topic_camera.c_str()) == 0) {
    if (!reader->m_beginToCalib) {
      LOG(ERROR) << "waiting for calibration ctrl";
    } else {
      minieye::CameraFrame camera;
      if (!camera.ParseFromArray(ptr, size)) {
        LOG(ERROR) << "parse proto CameraFrame failed";
      } else {
        reader->save_camera(camera);
      }
    }
  } else if (strcmp(topic, reader->m_dds_topic_ctrl.c_str()) == 0) {
    minieye::AVMCalibCtrl calib_ctrl;
    if (!calib_ctrl.ParseFromArray(ptr, size)) {
      LOG(ERROR) << "parse proto AVMCalibCtrl failed";
    } else {
      LOG(ERROR) << "ready to calib";
      google::protobuf::Map<int32_t, float> *google_map =
          calib_ctrl.mutable_signals();
      if (google_map->contains(minieye::AVMCalibCtrl::START_AUTO_CALIB)) {
        reader->setCalibCtrl();
      } else if (google_map->contains(minieye::AVMCalibCtrl::STOP_CALIB)) {
        reader->resetAvmCalibStatus();
      }
    }
  } else {
    LOG(ERROR) << "invalid topic: " << topic;
  }
}

unsigned long long GetCurrentTimeMsec() {
#ifdef _WIN32
  struct timeval tv;
  time_t clock;
  struct tm tm;
  SYSTEMTIME wtm;

  GetLocalTime(&wtm);
  tm.tm_year = wtm.wYear - 1900;
  tm.tm_mon = wtm.wMonth - 1;
  tm.tm_mday = wtm.wDay;
  tm.tm_hour = wtm.wHour;
  tm.tm_min = wtm.wMinute;
  tm.tm_sec = wtm.wSecond;
  tm.tm_isdst = -1;
  clock = mktime(&tm);
  tv.tv_sec = clock;
  tv.tv_usec = wtm.wMilliseconds * 1000;
  return ((unsigned long long)tv.tv_sec * 1000 +
          (unsigned long long)tv.tv_usec / 1000);
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((unsigned long long)tv.tv_sec * 1000 +
          (unsigned long long)tv.tv_usec / 1000);
#endif
}

int32_t appInit() {
  int32_t status = 0;

  status = appCommonInit();

  if (status == 0) {
    tivxInit();
    tivxHostInit();
  }
  return status;
}

int32_t appDeInit() {
  int32_t status = 0;

  tivxHostDeInit();
  tivxDeInit();
  appCommonDeInit();
  VLOG(0) << "TDA4 app deinit";
  return status;
}

bool AvmCalibService::init(const std::string &dds_config_file,
                           std::map<std::string, std::string> &topic_names,
                           std::map<int32_t, std::string> &intrins_files,
                           std::string &car_info_file, std::string &layout_file,
                           std::string &savedPath, float reproject_err) {
  m_beginToCalib.store(false);
  m_fReprojectErr = reproject_err;
  setDiagCode();
  mount("/dev/disk/by-partlabel/calib", "/calib", "ext4", MS_REMOUNT, NULL);
  m_savedPath = savedPath;
  std::string extrins_version_path = savedPath;
  extrins_version_path += "/version.json";
  m_statusFile = extrins_version_path;
  getVehicleCalibStatus(extrins_version_path);
  do {
    FILE *fid = fopen(dds_config_file.c_str(), "r");
    if (NULL == fid) {
      LOG(ERROR) << "faile to get config file: " << dds_config_file;
      CWrapperDiag::instance().report_error(CAPA_INPUT, FOBJECT_DDS,
                                            EAVMILOADCONFIGERROR);
      m_init_state = false;
      break;
    } else {
      LOG(INFO) << "success load dds context file";
    }
    fclose(fid);
    m_ctx = new minieye::DDS::Context(dds_config_file, true);

    m_dds_topic_camera = topic_names["camera"];
    m_dds_topic_ctrl = topic_names["ctrl_status"];
    Reader reader_camera(m_ctx, m_dds_topic_camera, onData, this);
    Reader reader_ctrl(m_ctx, m_dds_topic_ctrl, onData, this);
    m_avmStatusWriter =
        new minieye::DDS::Writer(m_ctx, topic_names["output_status"]);
    m_init_state = true;

    // init avm
    m_intrins_files.resize(4);
    char config[4][128];
    memset(config, 0, sizeof(config));
    for (std::map<int, std::string>::iterator it = intrins_files.begin();
         it != intrins_files.end(); ++it) {
      int32_t index = std::log2(it->first);
      std::string intrins_json = it->second;
      m_intrins_files[index] = intrins_json;

      memcpy(config[index], intrins_json.c_str(),
             intrins_json.size() * sizeof(char));
    }

    m_avmCalibHandle =
        initAvmCalib(config, layout_file.c_str(), car_info_file.c_str());
    if (nullptr == m_avmCalibHandle) {
      LOG(ERROR) << "faile to init avm calib handle";
      CWrapperDiag::instance().report_error(CAPA_INPUT, FAPA_LOAD_CONFIG,
                                            EAVMILOADCONFIGERROR);
      m_init_state = false;
      break;
    }

    int32_t status = appInit();
    if (status != 0) {
      LOG(ERROR) << "faile to init tda4 appCommon";
      CWrapperDiag::instance().report_error(CAPA_SYSTEM, FSOC_INIT,
                                            EUNREACHABLE);
      m_init_state = false;
      break;
    }

  } while (false);

  return m_init_state;
}

void AvmCalibService::resetAvmCalibStatus() {
  google::protobuf::Map<int32_t, int32_t> *google_map =
      m_avmState.mutable_states();
  for (uint32_t inum = 0; inum < AVM_CAMERA_NUM; ++inum) {
    int camera_id = 1u << inum;
    if (google_map->contains(camera_id)) {
      google_map->at(camera_id) = AvmCalibState::kReadyToCalib;
    } else {
      google_map->insert({camera_id, AvmCalibState::kReadyToCalib});
    }
    auto size = m_avmState.ByteSizeLong();
    char data[10240] = {0};
    m_avmState.SerializeToArray(data, size);
    m_avmStatusWriter->Write(data, size);
  }

  m_beginToCalib.store(false);
}

void AvmCalibService::uninit() {
  avmRelease(m_avmCalibHandle);
  delete m_ctx;
  appDeInit();
}

void AvmCalibService::read_data(cv::Mat &img, uint64_t &camera_time_ms) {
  std::unique_lock<std::mutex> lock(m_lock);
  m_cond_camera.wait(lock);
  camera_time_ms = m_camera_time_ms;
  getInputImage(&m_camera, img);
  lock.unlock();
}

void AvmCalibService::save_camera(minieye::CameraFrame camera) {
  std::unique_lock<std::mutex> lock(m_lock);
  m_camera_time_ms = GetCurrentTimeMsec();  // GetTimeStampInUs();
  m_camera = camera;
  m_cond_camera.notify_one();
  lock.unlock();
}

void AvmCalibService::setCalibCtrl() {
  std::unique_lock<std::mutex> lock(m_lock_ctrl);
  m_cond_ctrl.notify_one();
  lock.unlock();
}

void AvmCalibService::getCalibCtrl() {
  std::unique_lock<std::mutex> lock(m_lock_ctrl);
  m_cond_ctrl.wait(lock);
  m_beginToCalib.store(true);
  lock.unlock();
}

int AvmCalibService::getInputImage(const minieye::CameraFrame *camera,
                                   cv::Mat &bgr) {
  uint32_t width = camera->image_width();
  uint32_t height = camera->image_height();
  uint32_t y_size = width * height;
  uint32_t uv_size = y_size >> 1;

  uint64_t phy_addr_0 = camera->image_plane_addr(0);
  uint64_t phy_addr_1 = camera->image_plane_addr(1);

  void *data_ptr = nullptr, *data_ptr2 = nullptr;
  int32_t ret = 0;
  if (phy_addr_0 > 0 && phy_addr_1 > 0) {
    Tda4PhyToVirt(phy_addr_0, y_size, &data_ptr);
    Tda4PhyToVirt(phy_addr_1, uv_size, &data_ptr2);
  }

  if (nullptr == data_ptr || nullptr == data_ptr2) {
    LOG(ERROR) << "failed to convert phy to virt";
    ret = -1;
  } else {
    cv::Mat nv12(cv::Size(width, height * 3 / 2), CV_8UC1);
    memcpy(nv12.data, data_ptr, sizeof(unsigned char) * y_size);
    memcpy(nv12.data + y_size, data_ptr2, sizeof(unsigned char) * uv_size);
    cv::cvtColor(nv12, bgr, cv::COLOR_YUV2BGR_NV12);
  }

  return ret;
}

void AvmCalibService::run(AvmCalibService *ptr) {
  while (true) {
    if (!ptr->m_init_state) {
      LOG(ERROR) << "failed to init avm calib service";
      break;
    }
    ptr->resetAvmCalibStatus();
    uint64_t read_timestamp;
    if (!ptr->m_beginToCalib) {
      ptr->getCalibCtrl();
    }
    cv::Mat img;
    ptr->read_data(img, read_timestamp);
    if (img.empty()) {
      LOG(WARNING) << "empty image";
      continue;
    }
    LOG(WARNING) << "read camera frame timestamp: " << read_timestamp;

    if (ptr->m_beginToCalib) {
      uint32_t width = img.cols / 2;
      uint32_t height = img.rows / 2;
      std::vector<cv::Mat> input_imgs;
      // 组合图，左上--左
      input_imgs.push_back(img(cv::Rect(0, 0, width, height)).clone());
      // 组合图，右上--右
      input_imgs.push_back(img(cv::Rect(width, 0, width, height)).clone());
      // 组合图，左下--前
      input_imgs.push_back(img(cv::Rect(0, height, width, height)).clone());
      // 组合图，右下--后
      input_imgs.push_back(img(cv::Rect(width, height, width, height)).clone());

      google::protobuf::Map<int32_t, int32_t> *google_map =
          ptr->m_avmState.mutable_states();
      auto size = ptr->m_avmState.ByteSizeLong();
      char data[10240] = {0};
      int camera_ids[] = {4, 8, 1, 2};
      std::vector<std::string> direction_name = {"front", "rear", "left",
                                                 "right"};
      for (uint32_t inum = 0; inum < input_imgs.size(); ++inum) {
        usleep(100 * 1000);

        google_map->at(camera_ids[inum]) = (int)AvmCalibState::kCalibrating;
        memset(data, 0, sizeof(data));
        ptr->m_avmState.SerializeToArray(data, size);
        int ret = ptr->m_avmStatusWriter->Write(data, size);
        if (ret != DDSSuccess) {
          LOG(ERROR) << "failed to write dds";
          CWrapperDiag::instance().report_error(CAPA_OUTPUT, FAPA_DDS_ERROR,
                                                EDDSSEND);
        }
        int chn = (int)std::log2(camera_ids[inum]);
        std::string res_str = ptr->m_savedPath + direction_name[chn] + ".bak";
        float err = avmCalib(ptr->m_avmCalibHandle, camera_ids[inum],
                             input_imgs[inum].data, res_str.c_str());
        LOG(ERROR) << direction_name[inum] << " remapping err: " << err;
        if (err < ptr->m_fReprojectErr && err > 0 && ptr->m_beginToCalib) {
          google_map->at(camera_ids[inum]) = (int)AvmCalibState::kSuccess;
        } else {
          google_map->at(camera_ids[inum]) = (int)AvmCalibState::kFailure;
          if (err > ptr->m_fReprojectErr) {
            auto val_err = ptr->m_camera_calib_err[camera_ids[inum]];
            CWrapperDiag::instance().report_error(std::get<0>(val_err),
                                                  std::get<1>(val_err),
                                                  std::get<2>(val_err));
          } else if (AVM_ERROR_CONNER_NOT_DETECTED == std::abs(err)) {
            auto val_conner = ptr->m_camera_calib_conner[camera_ids[inum]];
            CWrapperDiag::instance().report_error(std::get<0>(val_conner),
                                                  std::get<1>(val_conner),
                                                  std::get<2>(val_conner));
          } else {
            auto val = ptr->m_camera_calib_saved[camera_ids[inum]];
            CWrapperDiag::instance().report_error(
                std::get<0>(val), std::get<1>(val), std::get<2>(val));
          }
        }

        memset(data, 0, sizeof(data));
        ptr->m_avmState.SerializeToArray(data, size);
        ret = ptr->m_avmStatusWriter->Write(data, size);
        if (ret != DDSSuccess) {
          LOG(ERROR) << "failed to write dds";
          CWrapperDiag::instance().report_error(CAPA_OUTPUT, FAPA_DDS_ERROR,
                                                EDDSSEND);
        }
        if (google_map->at(camera_ids[inum]) != AvmCalibState::kSuccess) {
          break;
        }
      }

      if (google_map->at(1) == AvmCalibState::kSuccess &&
          google_map->at(2) == AvmCalibState::kSuccess &&
          google_map->at(4) == AvmCalibState::kSuccess &&
          google_map->at(8) == AvmCalibState::kSuccess) {
        ptr->changeAttribut();
        ptr->writeCalibStatus(ptr->m_statusFile);
      }
    }
    usleep(100 * 1000);
  }
}

void AvmCalibService::changeAttribut() {
  std::vector<std::string> direction_name = {"front", "rear", "left", "right"};
  for (uint32_t inum = 0; inum < AVM_CAMERA_NUM; ++inum) {
    std::string result_file_name = m_savedPath + direction_name[inum] + ".json";

    std::string res_str = m_savedPath + direction_name[inum] + ".bak";

    std::string pipe_cmd1 = "chattr -i " + result_file_name;
    std::string pipe_cmd2 = "mv " + res_str + " " + result_file_name;
    std::string pipe_cmd3 = "chattr +i " + result_file_name;
    system(pipe_cmd1.c_str());
    system(pipe_cmd2.c_str());
    system(pipe_cmd3.c_str());
    sync();
  }
}

void AvmCalibService::getVehicleCalibStatus(const std::string &status_file) {
  std::ifstream fin(status_file);
  bool flag = false;
  if (fin.is_open()) {
    nlohmann::json root;
    fin >> root;
    if (root.find("bCalibStatus") != root.end()) {
      int status = root.at("bCalibStatus");
      if (1 == status) {
        flag = true;
      }
    }
  }
  fin.close();
  if (!flag) {
    CWrapperDiag::instance().report_error(CAPA_OUTPUT, FAVM_CALIB_STATUS,
                                          EAVMICAMNOTCALIB);
  } else {
    CWrapperDiag::instance().error_recover(CAPA_OUTPUT, FAVM_CALIB_STATUS,
                                           EAVMICAMNOTCALIB);
  }
}

void AvmCalibService::writeCalibStatus(const std::string &status_file) {
  nlohmann::json root;
  root["bCalibStatus"] = 1;

  static char chTime[64];
  struct timeval timeVal;
  gettimeofday(&timeVal, NULL);
  struct tm sTM;
  localtime_r(&timeVal.tv_sec, &sTM);
  sprintf(chTime, "%04d%02d%02d%02d%02d%02d", (sTM.tm_year + 1900),
          sTM.tm_mon + 1, sTM.tm_mday, sTM.tm_hour, sTM.tm_min, sTM.tm_sec);
  std::string strLocalTime = chTime;

  root["time"] = strLocalTime;

  // 输出到文件
  std::ofstream os;
  os.open(status_file, std::ios::out);
  os << root.dump(4) << std::endl;
  os.close();
  sync();

  CWrapperDiag::instance().error_recover(CAPA_OUTPUT, FAVM_CALIB_STATUS,
                                         EAVMICAMNOTCALIB);
}

void AvmCalibService::setDiagCode() {
  m_camera_calib_err[1] =
      std::make_tuple(CAPA_OUTPUT, FAVM_CALIB_STATUS, EAVMPROCAM1CALIBFAILED);
  m_camera_calib_err[2] =
      std::make_tuple(CAPA_OUTPUT, FAVM_CALIB_STATUS, EAVMPROCAM2CALIBFAILED);
  m_camera_calib_err[4] =
      std::make_tuple(CAPA_OUTPUT, FAVM_CALIB_STATUS, EAVMPROCAM3CALIBFAILED);
  m_camera_calib_err[8] =
      std::make_tuple(CAPA_OUTPUT, FAVM_CALIB_STATUS, EAVMPROCAM4CALIBFAILED);

  m_camera_calib_conner[1] =
      std::make_tuple(CAPA_PROCEDURE, FAVM_CALIB_STATUS, EAVMPRO1CHNOTCONNER);
  m_camera_calib_conner[2] =
      std::make_tuple(CAPA_PROCEDURE, FAVM_CALIB_STATUS, EAVMPRO2CHNOTCONNER);
  m_camera_calib_conner[4] =
      std::make_tuple(CAPA_PROCEDURE, FAVM_CALIB_STATUS, EAVMPRO3CHNOTCONNER);
  m_camera_calib_conner[8] =
      std::make_tuple(CAPA_PROCEDURE, FAVM_CALIB_STATUS, EAVMPRO4CHNOTCONNER);

  m_camera_calib_saved[1] = std::make_tuple(CAPA_PROCEDURE, FAVM_CALIB_STATUS,
                                            EAVMO1CHSAVECALIBFAILED);
  m_camera_calib_saved[2] = std::make_tuple(CAPA_PROCEDURE, FAVM_CALIB_STATUS,
                                            EAVMO2CHSAVECALIBFAILED);
  m_camera_calib_saved[4] = std::make_tuple(CAPA_PROCEDURE, FAVM_CALIB_STATUS,
                                            EAVMO3CHSAVECALIBFAILED);
  m_camera_calib_saved[8] = std::make_tuple(CAPA_PROCEDURE, FAVM_CALIB_STATUS,
                                            EAVMO4CHSAVECALIBFAILED);
}

}  // namespace avm
}  // namespace minieye
