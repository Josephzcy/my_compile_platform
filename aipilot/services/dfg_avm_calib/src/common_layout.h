//
// Created by minieye on 2022/6/21.
//

#ifndef AIPLORER_COMMON_LAYOUT_H
#define AIPLORER_COMMON_LAYOUT_H

#include "aipilot/services/dfg_avm_calib/src/calibration_layout.h"
#include "msg/precompiler/avm_calib_param.pb.h"

namespace minieye {
namespace avm {
typedef struct {
  float w;
  float h;
  float d;
  float v;
} CommonLayoutParam;

class CommonLayout : public CalibrationLayout {
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
  CommonLayoutParam layout_param_;
};
} // namespace avm
} // namespace minieye

#endif // AIPLORER_COMMON_LAYOUT_H
