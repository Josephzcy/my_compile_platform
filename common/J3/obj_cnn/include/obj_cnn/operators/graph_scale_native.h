// Copyright 2021 MINIEYE
//
// TDA4平台一系列图像抠图resize拼接操作在native上的实现

#ifndef INCLUDE_OBJ_CNN_OPERATORS_GRAPH_SCALE_NATIVE_H_
#define INCLUDE_OBJ_CNN_OPERATORS_GRAPH_SCALE_NATIVE_H_

#include <condition_variable>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"

namespace obj_cnn {

class BaseGraphScaleOperator : public Operator {
 public:
  BaseGraphScaleOperator();
  virtual ~BaseGraphScaleOperator();

  size_t GetWidth() { return dst_width_; }
  size_t GetHeight() { return dst_height_; }

 protected:
  void Init(int src_width, int src_height, int dst_width, int dst_height);
  int Output(const Image &image, std::vector<CvData *> const &output);

  int src_width_;
  int src_height_;
  int dst_width_;
  int dst_height_;
  int dst_span_col_;
  int dst_span_row_;
  cv::Mat dst_mat_;
};

// 输入3840*1080的图像，输出拼接1792*352的图像，前视2个roi
class GraphScale1792x352Operator : public BaseGraphScaleOperator {
 public:
  GraphScale1792x352Operator();

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "GraphScale1792x352"; }

  // 没有选项。
  int LoadOption(toml::value const &) override { return 0; }
};

// 前视2个roi+后视1个roi
class GraphScaleForDetectOperator : public BaseGraphScaleOperator {
 public:
  GraphScaleForDetectOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "GraphScaleForDetect"; }

  int LoadOption(toml::value const &) override;
};

// 单个roi缩放
class GraphScaleSingleRoiOperator : public BaseGraphScaleOperator {
 public:
  GraphScaleSingleRoiOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "GraphScaleSingleRoi"; }

  int LoadOption(toml::value const &) override;
};

// 输入3840*1080或2560*1440的图像，输出拼接288*288或384*384或480*480的图像
class GraphScaleGridOperator : public BaseGraphScaleOperator {
 public:
  static constexpr size_t SPAN_SIZE = 96;

  GraphScaleGridOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "GraphScaleGrid"; }

  int LoadOption(toml::value const &) override;

 private:
  int span_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_GRAPH_SCALE_NATIVE_H_
