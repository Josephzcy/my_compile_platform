//
// Created by minieye on 2022/6/22.
//

#ifndef AIPLORER_JX65_LAYOUT_H
#define AIPLORER_JX65_LAYOUT_H

#include "aipilot/services/dfg_avm_calib/src/calibration_layout.h"
#include "msg/precompiler/avm_calib_param.pb.h"
#include "opencv2/opencv.hpp"

namespace minieye {
namespace avm {
typedef struct {
  // 外部测量参数
  float w;
  float h;
  float d;
  float v;
  float w1;
  float w2;
} JX65LayoutParam;

typedef struct {
  float CW1;
  float CW2;
  float CW3;
  float CW4;
  float CW5;
} AvmExtraCheckerBoardInfo;

class JX65Layout : public CalibrationLayout {
public:
  int initLayout(const std::string &extrins_file,
                 const std::string &car_info_json) override;
  int getLayoutWorldPts(
      std::vector<std::vector<cv::Point3d>> &world_pts) override;
  /**
   * 根据通道号计算棋盘格角点世界坐标，front--0x1, rear--0x2, left--0x4,
   * right--0x8
   * @param camera_id
   * @param world_pt
   * @return
   */
  int getWorldPt(uint32_t camera_id,
                 std::vector<cv::Point3d> &world_pt) override;

  int getCheckerBoardConnerSize(cv::Size &conner_size) override;

  int getLayoutType() override { return layout_type_; }
  CarInfo getCarInfo() override { return car_info_; }

  /**
   * jx65项目特殊输出棋盘格额外信息，前后边角处的小棋盘格角点尺寸
   * @param extra_board_size
   * @param extra_conner
   * @return
   */
  int getExtraCheckerBoardInfo(
      cv::Size &extra_board_size,
      std::vector<std::vector<cv::Point3d>> &extra_conner);

private:
  bool parseExtrins(const std::string &extrins_file) override;

  void getFrontPt(std::vector<cv::Point3d> &front_world_pt) override;

  void getLeftPt(std::vector<cv::Point3d> &left_world_pt) override;

  void getRightPt(std::vector<cv::Point3d> &right_world_pt) override;

  void getRearPt(std::vector<cv::Point3d> &rear_world_pt) override;

  /**
   * 计算世界坐标点的时候，是以前左棋盘格的外边沿交点作为原点，建立北西天坐标系，
   * 这一步的作用是添加平移，改变参考点原点
   */
  void translate(const std::vector<cv::Point3d> &world_pts,
                 std::vector<cv::Point3d> &new_world_pts) override;

private:
  AvmCheckerBoardInfo checker_info_;
  AvmExtraCheckerBoardInfo extra_checker_info_;
  JX65LayoutParam layout_param_;

  float minD_;
  float maxD_;
};
} // namespace avm
} // namespace minieye

#endif // AIPLORER_JX65_LAYOUT_H
