// Copyright [2022] - MiniEye INC.
#include "dsp_stitcher.h"

#include <fstream>

#include "TI/tivx.h"
#include "app_init.h"
#include "app_mem.h"
#include "glog/logging.h"
#include "json.hpp"
extern "C" {
#include "minicv.h"
}

static void Tda4VirtToPhy(void *vir_addr, void **phy_addr) {
  uint64_t fd64;
  vx_status status = tivxMemTranslateVirtAddr(vir_addr, &fd64, phy_addr);
  if (status != (vx_status)VX_SUCCESS) {
    LOG(ERROR) << "tivxMemTranslateVirtAddr() failed for image";
  }
}

static void Tda4PhyToVirt(uint64_t phy_addr, uint32_t size, void **virt_addr) {
  void *phy_addr_tmp;
  uint32_t buf_fd;
  buf_fd = appMemGetDmaBufFdByPhyAddr(phy_addr);
  tivxMemTranslateFd((uint64_t)buf_fd, size, virt_addr, &phy_addr_tmp);
}

DspStitcher::~DspStitcher() {
  app_delete_graph_scale_2tda(MINICV_TARGET_DSP2);
  tivxMemFree(lut_data_vir_, lut_size_, TIVX_MEM_EXTERNAL);

  for (uint i = 0; i < FLAGS_bev_buffer_num; ++i) {
    for (int j = 0; j < 2; ++j) {
      int scale = (j == 0) ? 1 : 2;
      tivxMemFree(buffer_bev_[i].nv12_vir_addr[j], FLAGS_output_width * FLAGS_output_height / scale * sizeof(uint8_t),
                  TIVX_MEM_EXTERNAL);
    }
  }

  tivxHostDeInit();
  tivxDeInit();
  appCommonDeInit();
}

int DspStitcher::InitResources() {
  // openvx inition
  vx_status status = appCommonInit();
  if (status != VX_SUCCESS) {
    LOG(ERROR) << "app common init failed";
    return -1;
  }
  tivxInit();
  tivxHostInit();

  // allocate memory for bev
  buffer_bev_.resize(FLAGS_bev_buffer_num);
  for (uint i = 0; i < FLAGS_bev_buffer_num; i++) {
    for (int j = 0; j < 2; j++) {
      int scale = (j == 0) ? 1 : 2;
      int alloc_size = FLAGS_output_width * FLAGS_output_height / scale * sizeof(uint8_t);
      buffer_bev_[i].nv12_vir_addr[j] = tivxMemAlloc(alloc_size, TIVX_MEM_EXTERNAL);

      uint8_t init_value = (j == 0) ? 0 : 128;
      memset(buffer_bev_[i].nv12_vir_addr[j], init_value, alloc_size);
      LOG(INFO) << "allocate size " << alloc_size << ", addr: " << buffer_bev_[i].nv12_vir_addr[j] << ", set value to "
                << static_cast<int>(init_value);

      if (buffer_bev_[i].nv12_vir_addr[j] == nullptr) {
        LOG(ERROR) << "app_tidl: ERROR: Unable to allocate memory for inPlanes, size = "
                   << FLAGS_output_width * FLAGS_output_height / scale;
        return -1;
      }

      // get physical addr from virtual addr
      Tda4VirtToPhy(buffer_bev_[i].nv12_vir_addr[j], &(buffer_bev_[i].nv12_phy_addr[j]));
    }
  }

  // allocate memory for avmmap lut
  return LoadDspResource();
}

int DspStitcher::LoadDspResource() {
  // load lut data
  std::ifstream file(FLAGS_lut_file.c_str(), std::ios::binary | std::ios::ate);
  if (!file) {
    LOG(ERROR) << "Failed to open " << FLAGS_lut_file;
    return -1;
  }
  lut_size_ = static_cast<int>(file.tellg());
  file.seekg(0, std::ios::beg);

  lut_data_vir_ = tivxMemAlloc(lut_size_, TIVX_MEM_EXTERNAL);
  if (lut_data_vir_ == nullptr) {
    LOG(ERROR) << "app_tidl: ERROR: Unable to allocate memory for inPlanes, size = " << lut_size_;
    return -1;
  }
  Tda4VirtToPhy(lut_data_vir_, &lut_data_phy_);

  if (file.read(static_cast<char *>(lut_data_vir_), lut_size_)) {
    LOG(INFO) << "Loaded lut file: " << FLAGS_lut_file << ", size: " << lut_size_ << " bytes.";
  } else {
    LOG(ERROR) << "Failed to read file!";
    return -1;
  }
  file.close();

  memset(param_obj_, 0, sizeof(param_obj_));
  param_obj_[99] = 43;
  param_obj_[0] = FLAGS_width;
  param_obj_[1] = FLAGS_height;
  param_obj_[2] = FLAGS_output_width;
  param_obj_[3] = FLAGS_output_height;
  cv::Rect roi;
  GetCarRoi(roi);
  LOG(INFO) << "Final black rect roi: " << roi.x << ", " << roi.y << ", " << roi.width << ", " << roi.height;
  param_obj_[4] = roi.x;
  param_obj_[5] = roi.y;
  param_obj_[6] = roi.width;
  param_obj_[7] = roi.height;
  param_obj_[8] = reinterpret_cast<int64_t>(lut_data_phy_) & 0xffffffff;

  app_create_graph_scale_2tda(in_pic_vir_, param_obj_, sizeof(param_obj_), FLAGS_width, FLAGS_height,
                              buffer_bev_[cur_bev_idx_].nv12_vir_addr, FLAGS_output_width, FLAGS_output_height,
                              MINICV_INTERPOLATION_NEAREST_NEIGHBOR, MINICV_TARGET_DSP2);
  app_set_inImgPara_2tda(FLAGS_width, FLAGS_height, MINICV_TARGET_DSP2);
  app_set_outImgPara_2tda(FLAGS_output_width, FLAGS_output_height, MINICV_TARGET_DSP2);
  LOG(INFO) << "app_create_graph_scale_2tda success";

  return 0;
}

void DspStitcher::Update(const minieye::CameraFrame &camera) {
  TranslatePhyAddr(camera.image_plane_addr(0), camera.image_plane_addr(1), FLAGS_width, FLAGS_height, &in_pic_vir_[0],
                   &in_pic_vir_[1]);
  LOG(INFO) << "in_pic_vir_[0]: " << in_pic_vir_[0] << ", in_pic_vir_[1]: " << in_pic_vir_[1];

  if (!FLAGS_input_frame_dir.empty()) {
    cv::Mat img_bgr;
    cv::Mat mat_y(cv::Size(FLAGS_width, FLAGS_height), CV_8UC1, in_pic_vir_[0]);
    cv::Mat mat_uv(cv::Size(FLAGS_width / 2, FLAGS_height / 2), CV_8UC2, in_pic_vir_[1]);
    cv::cvtColorTwoPlane(mat_y, mat_uv, img_bgr, cv::COLOR_YUV2BGR_NV12);
    std::string in_pic_save_name = FLAGS_input_frame_dir + "/camera30_" + std::to_string(camera.tick()) + ".png";
    LOG(WARNING) << "save input camera30 to " << in_pic_save_name;
    cv::imwrite(in_pic_save_name, img_bgr);
  }
  app_run_graph_scale_2tda(in_pic_vir_, buffer_bev_[cur_bev_idx_].nv12_vir_addr, param_obj_, sizeof(param_obj_),
                           MINICV_TARGET_DSP2);
  app_wait_graph_scale_2tda(MINICV_TARGET_DSP2);
}

void DspStitcher::GetCarRoi(cv::Rect &car_roi) {
  nlohmann::json config_json;
  std::ifstream config_stream(FLAGS_car_info_file, std::ifstream::in);
  config_stream >> config_json;
  double car_length = config_json["car_length"];
  double car_width = config_json["car_width"];
  double car_front_overhang = config_json["car_front_overhang"];
  double car_wheelbase = config_json["car_wheelbase"];
  double car_front_blend = config_json["car_front_blend"];
  double car_left_blend = config_json["car_left_blend"];
  double car_rear_blend = config_json["car_rear_blend"];
  double car_right_blend = config_json["car_right_blend"];

  const double ratio = (FLAGS_output_width / (FLAGS_half_width_phycical_dis * 2.0));

  // 宽度方向要求对称
  car_roi.width = static_cast<int>(car_width * ratio);

  if (car_roi.width % 2 != 0) {
    car_roi.width += 1;
  }
  car_roi.x = (FLAGS_output_width - car_roi.width) / 2;

  int left_x_offset = static_cast<int>(car_left_blend * ratio);
  int right_x_offset = static_cast<int>(car_right_blend * ratio);
  if (left_x_offset != right_x_offset) {
    LOG(ERROR) << "car blind left not equal to right";
  }
  car_roi.x -= left_x_offset;

  if (car_roi.x % 2 != 0) {
    car_roi.x += 1;
  }
  car_roi.width = FLAGS_output_width - car_roi.x * 2;
  LOG(INFO) << "width update: " << car_roi.x << ", " << car_roi.y << ", " << car_roi.width << ", " << car_roi.height;

  // 高度方向不对称
  car_roi.height = static_cast<int>(car_length * ratio);

  car_roi.y = (FLAGS_output_height - car_roi.height + 1) / 2;

  int top_y_offset = static_cast<int>(car_front_blend * ratio);
  int bottom_y_offset = static_cast<int>(car_rear_blend * ratio);
  car_roi.y -= top_y_offset;
  car_roi.height += (top_y_offset + bottom_y_offset);

  if (car_roi.y % 2 != 0) {
    car_roi.y -= 1;
    car_roi.height += 1;
  }

  if (car_roi.height % 2 != 0) {
    car_roi.height += 1;
  }
  LOG(INFO) << "height update: " << car_roi.x << ", " << car_roi.y << ", " << car_roi.width << ", " << car_roi.height;
}

void DspStitcher::TranslatePhyAddr(uint64_t src_y, uint64_t src_uv, int img_w, int img_h, void **y_ptr, void **uv_ptr) {
  bool find_y = false, find_uv = false;

  std::map<uint64_t, void *>::iterator it;
  it = y_addr_map_.find(src_y);
  if (it != y_addr_map_.end()) {
    *y_ptr = it->second;
    find_y = true;
  }
  it = uv_addr_map_.find(src_uv);
  if (it != uv_addr_map_.end()) {
    *uv_ptr = it->second;
    find_uv = true;
  }

  if (!find_y || !find_uv) {
    uint32_t image_size[4];

    image_size[0] = img_w * img_h;
    image_size[1] = image_size[0] >> 1;

    LOG(INFO) << "addr src_y:" << src_y << " src_uv:" << src_uv << " img_w:" << img_w << " img_h:" << img_h;

    Tda4PhyToVirt(src_y, image_size[0], y_ptr);
    Tda4PhyToVirt(src_uv, image_size[1], uv_ptr);

    y_addr_map_.insert(std::pair<uint64_t, void *>(src_y, *y_ptr));
    uv_addr_map_.insert(std::pair<uint64_t, void *>(src_uv, *uv_ptr));
  }
}

void DspStitcher::SaveNv12ToPngFile(const std::string &file_name) {
  cv::Mat mat_y(cv::Size(FLAGS_output_width, FLAGS_output_height), CV_8UC1, buffer_bev_[cur_bev_idx_].nv12_vir_addr[0]);
  cv::Mat mat_uv(cv::Size(FLAGS_output_width / 2, FLAGS_output_height / 2), CV_8UC2,
                 buffer_bev_[cur_bev_idx_].nv12_vir_addr[1]);
  cv::Mat img_bgr;
  cv::cvtColorTwoPlane(mat_y, mat_uv, img_bgr, cv::COLOR_YUV2BGR_NV12);
  cv::imwrite(file_name, img_bgr);

  LOG(WARNING) << "writing to file [" << file_name << "] image of size " << FLAGS_output_width << " x "
               << FLAGS_output_height << " ... Done !!!";

  if (FLAGS_save_bev_nv12_format) {
    std::string bin_name = file_name.substr(0, file_name.size() - 4) + ".bin";
    std::ofstream out_bin(bin_name, std::ios::binary);
    out_bin.write(reinterpret_cast<const char *>(buffer_bev_[cur_bev_idx_].nv12_vir_addr[0]),
                  FLAGS_output_width * FLAGS_output_height);
    out_bin.write(reinterpret_cast<const char *>(buffer_bev_[cur_bev_idx_].nv12_vir_addr[1]),
                  FLAGS_output_width * FLAGS_output_height / 2);
    LOG(WARNING) << "save bev nv12 bin to " << bin_name;
    out_bin.close();
  }
}
