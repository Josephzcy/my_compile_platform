// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_TRACK_H_
#define INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_TRACK_H_

#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"

namespace obj_cnn {

class VehicleTrackOperator : public Operator {
 public:
  VehicleTrackOperator();
  virtual ~VehicleTrackOperator();

 private:
  struct TrackParams {
    float pad_w;
    float pad_h;
    float scale_x;
    float scale_y;

    cv::Rect2f src_rect;
    cv::Rect2f dst_roi;

    const float *match_scores;
    const float *wh;
    const float *cls_score;
  };

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "VehicleTrack"; }

  int LoadOption(toml::value const &) override;

  int64_t pre_track_id_;
  bool track_status_;
  bool init_pre_frame_;
  float min_track_rect_area_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_VEHICLE_TRACK_H_
