// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_SMALL_BIG_COMBINE_H_
#define INCLUDE_OBJ_CNN_OPERATORS_SMALL_BIG_COMBINE_H_

#include <memory>
#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"

namespace obj_cnn {
class SmallBigCombineOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  enum SmallBigType {
    kSmall,
    kBig,
    kUnknow,
  };

  enum SmallKeyPointCls {
    kSmallKeyPointEllipse,
    kSmallKeyPointOctagon,
    kSmallKeyPointRectangle,
    kSmallKeyPointRhombus,
    kSmallKeyPointTriangle,
    kSmallKeyPointTriangleInv,
    kSmallKeyPointInvalid,
  };

  enum BigKeyPointCls {
    kBigKeyPointInvalid,
    kBigKeyPointValid,
  };

  std::string name() const override { return "SmallBigCombine"; }

  // 没有选项。
  int LoadOption(toml::value const &) override { return 0; }
};
}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_SMALL_BIG_COMBINE_H_
