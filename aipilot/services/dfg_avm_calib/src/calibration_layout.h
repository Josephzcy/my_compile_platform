//
// Created by minieye on 2022/6/21.
//

#ifndef AIPLORER_CALIBRATION_LAYOUT_H
#define AIPLORER_CALIBRATION_LAYOUT_H
#include <vector>

#include "msg/precompiler/avm_calib_param.pb.h"
#include "opencv2/opencv.hpp"

namespace minieye {
namespace avm {
// 棋盘格信息
typedef struct {
  float checker_dw; // 棋盘格小方格宽度
  float checker_dh; // 棋盘格小方格长度
  float vert_num; // 棋盘格垂直方向内角点个数，无特殊说明，以短边为垂直方向
  float horize_num; // 棋盘格水平方向内角点个数
} AvmCheckerBoardInfo;

class CalibrationLayout {
public:
  virtual int initLayout(const std::string &extrins_file,
                         const std::string &car_info_json) = 0;
  virtual int
  getLayoutWorldPts(std::vector<std::vector<cv::Point3d>> &world_pts) = 0;
  /**
   * 根据通道号计算棋盘格角点世界坐标，front--0x1, rear--0x2, left--0x4,
   * right--0x8
   * @param camera_id
   * @param world_pt
   * @return
   */
  virtual int getWorldPt(uint32_t camera_id,
                         std::vector<cv::Point3d> &world_pt) = 0;

  virtual int getCheckerBoardConnerSize(cv::Size &conner_size) = 0;

  virtual int getLayoutType() = 0;

  virtual CarInfo getCarInfo() = 0;

protected:
  virtual bool parseExtrins(const std::string &extrins_file) = 0;

  virtual void getFrontPt(std::vector<cv::Point3d> &front_world_pt) = 0;

  virtual void getLeftPt(std::vector<cv::Point3d> &left_world_pt) = 0;

  virtual void getRightPt(std::vector<cv::Point3d> &right_world_pt) = 0;

  virtual void getRearPt(std::vector<cv::Point3d> &rear_world_pt) = 0;

  /**
   * 计算世界坐标点的时候，是以前左棋盘格的外边沿交点作为原点，建立北西天坐标系，
   * 这一步的作用是添加平移，改变参考点原点
   */
  virtual void translate(const std::vector<cv::Point3d> &world_pts,
                         std::vector<cv::Point3d> &new_world_pts) = 0;

protected:
  CarInfo car_info_;
  int layout_type_;
};
} // namespace avm
} // namespace minieye

#endif // AIPLORER_CALIBRATION_LAYOUT_H
