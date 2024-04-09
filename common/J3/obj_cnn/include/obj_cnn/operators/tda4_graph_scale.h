// Copyright 2021 MINIEYE
// TDA4平台一系列图像抠图resize拼接操作

#ifndef INCLUDE_OBJ_CNN_OPERATORS_TDA4_GRAPH_SCALE_H_
#define INCLUDE_OBJ_CNN_OPERATORS_TDA4_GRAPH_SCALE_H_

#include <condition_variable>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <vector>
#include <string>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"
#include "obj_cnn/operator_graph_context.h"

namespace obj_cnn {

enum GraphScaleType {
  // 无效类别
  kGraphScaleTypeInvalid = 0,
  // 前后视3840x1080的检测
  kGraphScaleType3200x352 = 1,
  kGraphScaleType480x480,
  kGraphScaleType384x384,
  kGraphScaleType960x192,
  // 2560x1440的检测
  kGraphScaleType1280x704,
  kGraphScaleTypeYUV420sp2planer,
  kGraphScaleType2368x352,
  kGraphScaleType1920x192,
  // 1920*544 -> 448*128
  kGraphScaleType448x128,
  kGraphScaleType960x192_I420,
  kGraphScaleType1280x704_I420,
  kGraphScaleType480x480_I420,
  kGraphScaleType384x384_I420,
  kGraphScaleType2368x352_I420,
  kGraphScaleType960x192_YUV444,
  kGraphScaleType1280x704_YUV444,
  kGraphScaleType480x480_YUV444,
  kGraphScaleType384x384_YUV444,
  kGraphScaleType2368x352_YUV444,
  kGraphScaleType800x288_YUV444,
  // 模式21根据配置参数，可以输出三种图形
  kGraphScaleTypeMode21_YUV444,
  kGraphScaleType288x288_YUV444,
  kGraphScaleTypeForDetect_YUV444,  // full det
  kGraphScalePadding_YUV444,        // track
};

class BaseTDA4GraphScaleOperator : public Operator {
 public:
  BaseTDA4GraphScaleOperator();
  virtual ~BaseTDA4GraphScaleOperator();

  size_t GetWidth() { return dst_width_; }
  size_t GetHeight() { return dst_height_; }

 protected:
  void Init(int src_width, int src_height, int dst_width, int dst_height);
  void InitMinicvTarget();
  int Run(const Image &image, int32_t *paramObj, int32_t objlen,
          std::vector<CvData *> const &output);

  static std::mutex graph_mutex_;
  static cv::Size last_src_size_;
  static cv::Size last_dst_size_;

  int src_width_;
  int src_height_;
  int dst_width_;
  int dst_height_;
  int dst_span_col_;
  int dst_span_row_;
  size_t buffer_size_;
  uint8_t *dst_buffer_ = nullptr;

  OperatorGraphContext *context_;

  uint8_t target_dsp_;
};

// 前视2个roi+后视1个roi
class TDA4GraphScaleForDetectOperator : public BaseTDA4GraphScaleOperator {
 public:
  TDA4GraphScaleForDetectOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "TDA4GraphScaleForDetect"; }

  int LoadOption(toml::value const &) override;

  int LoadOptionWithContext(OperatorGraphContext *context,
                            toml::value const &) override;

  void Init(int src_width, int src_height, int dst_width, int dst_height,
            OperatorGraphContext *context = nullptr);
  int Run(const Image &image, int32_t *paramObj, int32_t objlen,
          std::vector<CvData *> const &output);

 private:
  enum class Layout {
    Horizontal,
    Vertical
  };
  Layout output_layout_;

  int LoadOptionLayout(toml::value const &);
};

// 单个roi缩放
class TDA4GraphScaleSingleRoiOperator : public BaseTDA4GraphScaleOperator {
 public:
  TDA4GraphScaleSingleRoiOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "TDA4GraphScaleSingleRoi"; }

  int LoadOption(toml::value const &) override;

  int LoadOptionWithContext(OperatorGraphContext *context,
                            toml::value const &) override;

  int Run(const Image &image, int32_t *paramObj, int32_t objlen,
          std::vector<CvData *> const &output);
};

// 单个roi缩放，补黑边
class TDA4GraphScaleBlackEdgingOperator : public BaseTDA4GraphScaleOperator {
 public:
  TDA4GraphScaleBlackEdgingOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "TDA4GraphScaleBlackEdging"; }

  int LoadOption(toml::value const &) override;
};

// 输入3840*1080或2560*1440的图像，输出拼接288*288或384*384或480*480的图像
class TDA4GraphScaleGridOperator : public BaseTDA4GraphScaleOperator {
 public:
  static constexpr size_t SPAN_SIZE = 96;

  TDA4GraphScaleGridOperator() {}

  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "TDA4GraphScaleGrid"; }

  int LoadOption(toml::value const &) override;

  int LoadOptionWithContext(OperatorGraphContext *context,
                            toml::value const &) override;

  int Run(const Image &image, int32_t *paramObj, int32_t objlen,
          std::vector<CvData *> const &output);

  GraphScaleType GetMaxPatchMode(int span_col, int span_row);

 private:
  enum class ScaleParamIndex {
    kPadL = 75,
    kPadR = 76,
    kPadF = 77,
    kPadB = 78,
    kMode = 99,
  };

  int span_;
  GraphScaleType mode_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_TDA4_GRAPH_SCALE_H_
