// Copyright [2022] - MiniEye INC.
#ifndef BEV_SRC_DSP_STITCHER_H_
#define BEV_SRC_DSP_STITCHER_H_

#include "aipilot/services/bird_eye_view/src/global.h"
#include "msg/precompiler/camera.pb.h"
#include "opencv2/opencv.hpp"

struct BevBuffer {
  void *nv12_phy_addr[2];
  void *nv12_vir_addr[2];
};

class DspStitcher {
 public:
  DspStitcher() = default;

  ~DspStitcher();

  DspStitcher(const DspStitcher &) = delete;

  const DspStitcher &operator=(const DspStitcher &) = delete;

  int InitResources();

  void Update(const minieye::CameraFrame &camera);

  void IncCurBevIdx() { cur_bev_idx_ = (cur_bev_idx_ + 1) % FLAGS_bev_buffer_num; }

  void SaveNv12ToPngFile(const std::string &file_name);

  const BevBuffer &FetchCurrentBev() const { return buffer_bev_[cur_bev_idx_]; };

  int cur_bev_idx() const { return cur_bev_idx_; }

 private:
  int LoadDspResource();
  static void GetCarRoi(cv::Rect &car_roi);

  void TranslatePhyAddr(uint64_t src_y, uint64_t src_uv, int img_w, int img_h, void **y_ptr, void **uv_ptr);

  bool init_ = false;
  std::vector<BevBuffer> buffer_bev_;
  int cur_bev_idx_ = 0;
  int lut_size_ = 0;
  int32_t param_obj_[100] = {};
  void *lut_data_phy_ = nullptr;
  void *lut_data_vir_ = nullptr;
  void *in_pic_vir_[2] = {nullptr, nullptr};
  std::map<uint64_t, void *> y_addr_map_;
  std::map<uint64_t, void *> uv_addr_map_;
};

#endif  // BEV_SRC_DSP_STITCHER_H_
