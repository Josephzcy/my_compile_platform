// Copyright [2022] - MiniEye INC.
#ifndef BEV_SRC_DDS_PROCESSOR_H_
#define BEV_SRC_DDS_PROCESSOR_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

#include "aipilot/services/bird_eye_view/src/global.h"
#include "dds_api.h"
#include "dds_common.h"
#include "msg/precompiler/apa_state.pb.h"
#include "msg/precompiler/camera.pb.h"
#include "msg/precompiler/vehicle_signal.pb.h"

class DdsProcessor {
 public:
  explicit DdsProcessor(std::string config_file, std::string dds_topic_camera, std::string output_dds_topic_camera);
  ~DdsProcessor();
  DdsProcessor(const DdsProcessor &) = delete;
  const DdsProcessor &operator=(const DdsProcessor &) = delete;

  bool HasData() { return m_has_camera_image; }
  std::atomic<bool> m_has_camera_image;

  bool door_is_closed() const { return door_is_closed_ || FLAGS_is_init_door_close; }

  bool is_in_apa_state() const { return is_in_apa_state_ || FLAGS_is_init_in_apa_state; }

  void set_door_is_closed(bool flag) { door_is_closed_.store(flag); }

  void set_is_in_apa_state(bool flag) { is_in_apa_state_.store(flag); }
  /*
@brief: read data, read saved data
@param camera[out]: camera data from dds
*/
  int ReadData(minieye::CameraFrame &camera);

  void ClearBevQueue();

  /*
 @brief: save data coming from dds
 @param camera[in]: camera data from dds
 */
  void SaveCamera(minieye::CameraFrame camera);

  /*
 @brief: send out dds topic
 @param camera[in]: use its timestamp and tick
 @param phy_addr_plane0[in]: phyAddr plane0
 @param phy_addr_plane1[in]: phyAddr plane1
 */
  int SendBevImage(const minieye::CameraFrame &camera, uint64_t phy_addr_plane0, uint64_t phy_addr_plane1, int idx);

  std::string dds_topic_camera() const { return dds_topic_camera_; }

 private:
  std::atomic<bool> door_is_closed_;
  std::atomic<bool> is_in_apa_state_;
  std::string dds_topic_camera_;
  std::string output_dds_topic_camera_;

  // make sure to process only when dds data arrived
  std::mutex lock_{};
  std::condition_variable cond_;
  std::queue<minieye::CameraFrame> camera_;

  minieye::DDS::Context ctx_;
  minieye::DDS::Writer *writer_bev_image_;
  minieye::CameraFrame out_msg_;
  char serialize_buf_[1024] = {0};
};

#endif  // BEV_SRC_DDS_PROCESSOR_H_
