// Copyright [2022] - MiniEye INC.
#include "aipilot/services/bird_eye_view/src/dds_processor.h"

#include <math.h>

#include "aipilot/services/bird_eye_view/src/wrapper_diag.h"
#include "glog/logging.h"

// @param topic: topic
// @param index: 内部数组的下标，array[index]
// @param ptr: 是指向当前元素的指针
// @param size: 当前元素的大小（字节）
// @param user: 用户数据
void OnData(const char *topic, size_t index, void *ptr, size_t size, void *user) {
  // LOG(INFO) << "[topic: " << topic << "] array[" << index << "], size="<< size;

  DdsProcessor *reader = reinterpret_cast<DdsProcessor *>(user);

  if (strcmp(topic, reader->dds_topic_camera().c_str()) == 0) {
    minieye::CameraFrame camera;
    if (!camera.ParseFromArray(ptr, size)) {
      CWrapperDiag::instance().ReportError(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVICAMNOTDATA);
      LOG(ERROR) << "parse proto CameraFrame failed";
      return;
    }
    if (!reader->door_is_closed()) {
      LOG(WARNING) << "Door is open, dds data will not be saved";
      return;
    }
    if (!reader->is_in_apa_state()) {
      return;
    }
    reader->SaveCamera(camera);

    reader->m_has_camera_image.store(true);
  } else if (strcmp(topic, FLAGS_dds_vehicle_signal_topic.c_str()) == 0) {
    minieye::VehicleSignal vs;
    if (!vs.ParseFromArray(ptr, size)) {
      CWrapperDiag::instance().ReportError(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVICAMNOTDATA);
      LOG(ERROR) << "parse proto VehicleSignal failed";
      return;
    }

    const google::protobuf::Map<int, float> &vs_map = vs.signals();
    if (std::fabs(vs_map.at(49) - 0.0) < 1e-6 && std::fabs(vs_map.at(50) - 0.0) < 1e-6) {
      if (!reader->door_is_closed()) {
        reader->ClearBevQueue();
        LOG(WARNING) << "Door was open, drop uneffective dds data";
      }
      reader->set_door_is_closed(true);
    } else if (std::fabs(vs_map.at(49) - 1.0) < 1e-6 || std::fabs(vs_map.at(50) - 1.0) < 1e-6) {
      LOG(INFO) << "front left door is_open: " << vs_map.at(49) << ", front right door is_open: " << vs_map.at(50);
      reader->set_door_is_closed(false);
    } else {
      CWrapperDiag::instance().ReportError(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVICAMNOTDATA);
      LOG(ERROR) << "Unknown door status!";
    }
  } else if (strcmp(topic, FLAGS_dds_apa_state_control_topic.c_str()) == 0) {
    minieye::APAStateControl apa_state;
    if (!apa_state.ParseFromArray(ptr, size)) {
      CWrapperDiag::instance().ReportError(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVICAMNOTDATA);
      LOG(ERROR) << "parse proto apa_state_control failed";
      return;
    }
    if (apa_state.pilot_apa_mode() == 2) {
      if (!reader->is_in_apa_state()) {
        reader->ClearBevQueue();
        LOG(WARNING) << "Check in apa state, drop uneffective dds data";
      }
      reader->set_is_in_apa_state(true);
    } else {
      reader->set_is_in_apa_state(false);
    }
  } else {
    CWrapperDiag::instance().ReportError(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVICAMNOTDATA);
    LOG(ERROR) << "invalid topic: " << topic;
  }
}

DdsProcessor::DdsProcessor(std::string config_file, std::string dds_topic_camera, std::string output_dds_topic_camera)
    : ctx_(config_file.c_str(), true) {
  dds_topic_camera_ = dds_topic_camera;
  output_dds_topic_camera_ = output_dds_topic_camera;

  minieye::DDS::Reader reader_camera(&ctx_, dds_topic_camera_.c_str(), OnData, this);
  minieye::DDS::Reader reader_vehicle_signal(&ctx_, FLAGS_dds_vehicle_signal_topic.c_str(), OnData, this);
  minieye::DDS::Reader reader_as(&ctx_, FLAGS_dds_apa_state_control_topic.c_str(), OnData, this);

  // todo: change into CameraFrame Writer
  writer_bev_image_ = new minieye::DDS::Writer(&ctx_, output_dds_topic_camera_.c_str());

  m_has_camera_image.store(false);
  door_is_closed_.store(false);
  is_in_apa_state_.store(false);
}

DdsProcessor::~DdsProcessor() { delete writer_bev_image_; }

int DdsProcessor::ReadData(minieye::CameraFrame &camera) {
  std::unique_lock<std::mutex> lock(lock_);
  if (camera_.empty()) {
    return -1;
  }
  // LOG(INFO) << "buffer num: " << camera_.size();
  camera = std::move(camera_.front());
  camera_.pop();
  lock.unlock();
  return 0;
}

void DdsProcessor::ClearBevQueue() {
  std::unique_lock<std::mutex> lock(lock_);
  while (!camera_.empty()) {
    camera_.pop();
  }
  lock.unlock();
}

void DdsProcessor::SaveCamera(minieye::CameraFrame camera) {
  std::unique_lock<std::mutex> lock(lock_);
  camera_.push(camera);
  if (camera_.size() > 1) {
    camera_.pop();
    // LOG(WARNING) << "Buffer full, drop raw camera dds data";
  }
  lock.unlock();
}

// todo: change into write CameraFrame
int DdsProcessor::SendBevImage(const minieye::CameraFrame &camera, uint64_t phy_addr_plane0, uint64_t phy_addr_plane1,
                               int idx) {
  out_msg_.set_camera_id(0);
  uint32_t stride = FLAGS_output_width;
  out_msg_.set_image_width(stride);
  out_msg_.set_image_height(FLAGS_output_height);
  out_msg_.set_stride(stride);
  out_msg_.set_image_type(2);
  out_msg_.clear_image_plane_addr();
  out_msg_.add_image_plane_addr(phy_addr_plane0);
  out_msg_.add_image_plane_addr(phy_addr_plane1);
  out_msg_.set_timestamp(camera.timestamp());
  out_msg_.set_tick(camera.tick());
  out_msg_.set_frame_id(camera.frame_id());
  out_msg_.set_image_supplement(&idx, sizeof(int));

  int csize = out_msg_.ByteSizeLong();
  if (!out_msg_.SerializeToArray(serialize_buf_, csize)) {
    CWrapperDiag::instance().ReportError(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVODDSERROR);
    LOG(ERROR) << "Fail to Serialize dds msg";
    return -1;
  }

  int ret = writer_bev_image_->Write(serialize_buf_, csize);

  if (ret != DDSSuccess) {
    CWrapperDiag::instance().ReportError(CAPA_PROCEDURE, FAPA_BEV_FPS_TOO_LOW, EBEVODDSERROR);
    LOG(ERROR) << "Write failed, ret = " << ret;
    return -1;
  }

  return 0;
}
