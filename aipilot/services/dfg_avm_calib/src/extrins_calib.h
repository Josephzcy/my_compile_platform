/******************************************************
    Author      : wangnb
    Date        : 2022/4/13
    Description :
        1.
    -----------------------------
    Change      :
        1. create ExtrinsCalib.h
*******************************************************/
#ifndef EXTRINS_CALIB_H
#define EXTRINS_CALIB_H

#include <cstdint>
#include <string>

#include "aipilot/services/dfg_avm_calib/src/calibration_layout.h"
#include "aipilot/services/dfg_avm_mapping/include/avm_mapping/avm_mapping.h"
#include "msg/precompiler/avm_calib_param.pb.h"
#include "opencv2/opencv.hpp"

namespace minieye {
namespace avm {
class AvmExtrinsCalib {
public:
  /**
   * 根据标定信息与相机内参（映射关系）初始化
   * @param calibInfo
   * @return
   */
  AvmErrorCode init(const std::vector<std::string> &intrins_file,
                    const std::string &layout_file,
                    const std::string &car_info_file);

  /**
   * 根据输入的图像id以及对应的标定图片进行标定，最后将外参结果进行存储
   * @param camera_id，front--0x1, rear--0x2, left--0x4, right--0x8
   * @param img，输入图像
   * @param calib_json，保存的json文件名字
   * @return
   */
  AvmErrorCode calib(CamDirect camera_id, const unsigned char *img_data,
                     const std::string &calib_json, float &remap_residual);

private:
  AvmErrorCode initCameraModel(const std::string &intrins_str);

  AvmErrorCode initAvmLayout(const std::string &extrins_str,
                             const std::string &car_info_str);

  /**
   * 将获取的外参按照说给文件名进行存储，其中R表示euler角
   * 度数，T表示相机位置，单位m
   * @param chn，表示相机通道，log2(camera_id)
   * @param extrins_json，外参文件
   * @param H，对应单应性矩阵
   * @param rvec，旋转向量
   * @param tvec，平移向量
   * @return
   */
  int32_t writeJson(uint32_t chn, const std::string &extrins_json,
                    const cv::Mat &H, const cv::Mat &rvec, const cv::Mat &tvec);

  /**
   * 用来检测棋盘格角点，并进行亚像素优化
   * @param img
   * @param conner_pts
   * @return
   */
  AvmErrorCode detectConner(const cv::Mat &img, const cv::Size &boardSize,
                            std::vector<cv::Point2d> &conner_pts);

  /**
   *根据像素坐标与世界坐标的关系计算外参RT，
   * @param chn
   * @param world_pts
   * @param pix_pts
   * @param rvec，旋转向量
   * @param tvec，平移向量
   * @param H, 返回的单应性矩阵
   */
  void calcExtrins(uint32_t chn, const std::vector<cv::Point3d> &world_pts,
                   const std::vector<cv::Point2d> &pix_pts, cv::Mat &rvec,
                   cv::Mat &tvec, cv::Mat &H, bool useExtrinsGusse = false);

  void undistImg(uint32_t chn, const cv::Mat &src_img, cv::Mat &undist_img);

  float getResidual(uint32_t chn, const std::vector<cv::Point3d> &world_pts,
                    const std::vector<cv::Point2d> &pix_pts,
                    const cv::Mat &rvec, const cv::Mat &tvec);

  // extrins refinement
  int refineExtrins(int chn, const cv::Mat &img, cv::Mat &rvec, cv::Mat &tvec,
                    cv::Mat &H, std::vector<cv::Point3d> &world_pts,
                    std::vector<cv::Point2d> &pix_pts);

  int project(int chn, const cv::Mat &rvec, const cv::Mat &tvec,
              const cv::Point3d &pw, cv::Point2d &proj_pt);

  int unproject(int chn, const cv::Mat &rvec, const cv::Mat &tvec,
                const cv::Point2d &pix, cv::Point3d &pw);

  void remapping(int chn, const cv::Mat &rvecs, const cv::Mat &tvecs,
                 const cv::Mat &imgs, cv::Mat &bev_imgs,
                 cv::Rect assistant_rois[2]);

  /**
   * 根据角点坐标进行顺时针排序
   * @param conners
   */
  void reorderConner(std::vector<cv::Point2d> &conners);
  /**
   * 判断jx65棋盘格排列方式，从而对应的进行重排
   * @param conners
   * @return
   */
  int getOrderMode(const std::vector<cv::Point2d> &conners);

  /**
   * 检测鸟瞰图上对角处的棋盘格角点
   * @param chn
   * @param bev_imgs
   * @param assistant_rois，额外棋盘格大致区域
   * @param allConners，提取的角点
   */
  void getAllConner(int chn, const cv::Mat &bev_imgs,
                    const cv::Rect assistant_rois[2],
                    std::vector<cv::Point2d> &allConners);

  /**
   * 前后可以看到清晰左右小棋盘格，此处直接进行检测，不经过鸟瞰图
   * @param chn
   * @param img
   * @param allConners
   */
  void getFrontRearExtraConner(int chn, const cv::Mat &img,
                               std::vector<cv::Point2d> &allConners);

  void bevPix2Vehicle(const cv::Point2d &bev_pt, cv::Point3d &vehicle_pt);

  /**
   * 为了控制棋盘格检测顺序，是直接检测对角处的小棋盘格，
   * 此函数是用来将角点变换到全局鸟瞰图上对应的点
   * @param img_size
   * @param assistan_size
   * @param all_conners
   */
  void transPoint(int chn, int sub_cb, const cv::Size &assistan_size,
                  std::vector<cv::Point2d> &all_conners);

private:
  cv::Size img_size_;
  cv::Size extra_CBSize_;
  std::vector<std::vector<cv::Point3d>> extra_CBPoints_;
  std::vector<minieye::CameraParam> camera_params_;
  std::shared_ptr<CalibrationLayout> avm_layout_;
  std::vector<cv::Mat> Ks_;

private:
  AVMMapper avm_mapper_;
};
} // namespace avm
} // namespace minieye

#endif // EXTRINS_CALIB_H