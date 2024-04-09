// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_BUFFER_FILL_H_
#define INCLUDE_OBJ_CNN_OPERATORS_BUFFER_FILL_H_

#include <string>
#include <vector>

#include "obj_cnn/operator.h"

namespace obj_cnn {

// 将多个 Buffer 拼接为一个更大的 Buffer，支持横向/纵向拼接。
class BufferFillOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  enum Mode {
    kFlatten,
    kGrid,
  };

  struct CellOption {
    int begin;  // 拼接到的位置，相对于buffer起始位置
    int width;
    int height;
    int stride;
  };

  std::string name() const override { return "BufferFill"; }

  // mode: flatten | grid
  // flatten 模式
  //   total-buffer-size: 总buffer大小
  //   cells: 每个buffer的begin, width, height, stride
  // grid 模式，每个格子宽高一致，stride就是拼接后的图的stride
  //   cell-width: 单个格子宽
  //   cell-height: 单个格子高
  //   cell-count: 总格子数量
  //   total-width: 总宽度
  //   total-height: 总高度
  int LoadOption(toml::value const &conf) override;

 private:
  Mode mode_;
  std::vector<uint8_t> buffer_;
  std::vector<CellOption> cells_;
  size_t total_buffer_size_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_BUFFER_FILL_H_
