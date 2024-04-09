// Copyright [2022] - MiniEye INC.
#include <csignal>
#include <string>
#include <thread>

#include "aipilot/services/bird_eye_view/src/dds_processor.h"
#include "aipilot/services/bird_eye_view/src/dsp_stitcher.h"
#include "aipilot/services/bird_eye_view/src/global.h"
#include "aipilot/services/bird_eye_view/src/version.h"
#include "aipilot/services/bird_eye_view/src/wrapper_diag.h"
#include "aipilot/services/bird_eye_view/utils/common_utils.h"
#include "aipilot/services/bird_eye_view/utils/fps.h"
#include "glog/logging.h"
#include "msg/precompiler/camera.pb.h"

int quit = 0;

void SigHandler(int signal) {
  LOG(INFO) << "Caught signal " << signal << ", setting flaq to quit.";
  quit = 1;
}

void ProcessSignal() {
  signal(SIGINT, SigHandler);   // ctrl + C
  signal(SIGTERM, SigHandler);  // kill command
  signal(SIGHUP, SigHandler);
  signal(SIGUSR1, SigHandler);
  signal(SIGQUIT, SigHandler);
}

int ProcessCore() {
  std::shared_ptr<DspStitcher> display = std::make_shared<DspStitcher>();

  int init_ret = display->InitResources();
  if (0 != init_ret) {
    CWrapperDiag::instance().ReportError(CAPA_PROCEDURE, FAPA_OPENGL_ERROR, EBEVOPENVXERROR);
    LOG(ERROR) << "fail to init dsp_stitcher resources!";
    return -1;
  }

  // 接收图片数据封装
  std::shared_ptr<DdsProcessor> p_dds_processor =
      std::make_shared<DdsProcessor>(FLAGS_dds_config, FLAGS_dds_image_topic, FLAGS_output_dds_image_topic);

  // fps 帧率计算
  minieye::fps::CFPS frame_fps("Bird's eye view");

  int64_t tm_start = common_utils::GetTimeMs();
  int64_t tm_pre = tm_start;
  int64_t gap_ms = static_cast<int>(1000.0 / FLAGS_max_fps);
  LOG(INFO) << "gap_ms: " << gap_ms;
  // don't send first render frame to inference as it caused error
  bool is_first_render = true;

  while (!quit) {
    if (!p_dds_processor->is_in_apa_state()) {
      LOG(INFO) << "not in APA state!";
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
      is_first_render = true;
      continue;
    }

    if (!p_dds_processor->door_is_closed()) {
      LOG(INFO) << "front door is open";
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
      is_first_render = true;
      continue;
    }

    if (!p_dds_processor->HasData()) {
      LOG(INFO) << "no dds data input";
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
      is_first_render = true;
      continue;
    }

    // 读取图片数据
    minieye::CameraFrame camera;
    if (p_dds_processor->ReadData(camera) < 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      LOG(WARNING) << "no data in dds queue!";
      continue;
    }

    // 打印 fps
    frame_fps.Triger();

    display->Update(camera);
    if (!FLAGS_output_frame_dir.empty()) {
      std::string str_frame_id;
      if (FLAGS_save_timestamp) {
        str_frame_id = std::to_string(camera.timestamp());
      } else {
        str_frame_id = std::to_string(camera.tick() * 1000);
      }
      std::string frame_file_name = FLAGS_output_frame_dir + "/" + str_frame_id + ".png";
      display->SaveNv12ToPngFile(frame_file_name);
    }

    if (!is_first_render) {
      int ret = p_dds_processor->SendBevImage(
          camera, reinterpret_cast<uint64_t>(display->FetchCurrentBev().nv12_phy_addr[0]),
          reinterpret_cast<uint64_t>(display->FetchCurrentBev().nv12_phy_addr[1]), display->cur_bev_idx());
      if (ret != DDSSuccess) {
        LOG(ERROR) << "write dds failed, " << FLAGS_output_dds_image_topic;
      }
    }

    display->IncCurBevIdx();

    if (is_first_render) {
      is_first_render = false;
    }

    int64_t tm_now = common_utils::GetTimeMs();
    int64_t dt_to_pre = tm_now - tm_pre;
    if (dt_to_pre < 0) {
      dt_to_pre = 0;
    }
    if (dt_to_pre < gap_ms) {
      int64_t diff = gap_ms - dt_to_pre;
      std::this_thread::sleep_for(std::chrono::milliseconds(diff));
    }
    tm_pre = common_utils::GetTimeMs();
  }

  return 0;
}

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::SetStderrLogging(google::GLOG_INFO);
  google::InstallFailureSignalHandler();
  FLAGS_colorlogtostderr = true;
  FLAGS_logtostderr = true;
  FLAGS_logbufsecs = 0;
  FLAGS_max_log_size = 1024;
  LOG(INFO) << "Version: " << _RELEASE_VERSION_ << "." << _MAJOR_VERSION_ << "." << _MINOR_VERSION_;
  ProcessSignal();

  int ret = ProcessCore();
  if (ret != 0) {
    return ret;
  }

  return 0;
}
