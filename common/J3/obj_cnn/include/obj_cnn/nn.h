// Copyright 2021 MINIEYE
//
// NeuralNetwork is a facade of platform specified network accelerator.

#ifndef INCLUDE_OBJ_CNN_NN_H_
#define INCLUDE_OBJ_CNN_NN_H_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "obj_cnn/buffer.h"
#include "obj_cnn/ndarray.h"
#include "obj_cnn/operator_graph_context.h"
#include "obj_cnn/type.h"
#include "toml/value.hpp"

namespace obj_cnn {

// NeuralNetwork transform buffers to buffers.
//
// You are not supposed to use this interface if you are not a author of CvData
// filters or combinators.
class NeuralNetwork : public Move {
 public:
  class Impl;

  NeuralNetwork();
  ~NeuralNetwork() {}

  explicit NeuralNetwork(std::shared_ptr<Impl> impl) : impl_(impl) {}

  // 加载配置参数。
  int Load(OperatorGraphContext *context, toml::value const &conf);

 private:
  // 加载跨平台的参数。
  //
  // o  input
  // 数组是整数数组，数组的长度是网络的输入数，数组的每一项是输入数据
  //    的字节数。
  // o  output 类似地描述了网络输出。
  int LoadCommon(toml::value const &conf);

  // 加载平台相关的选项。
  //
  // native/xenial49 等基于 caffe 的平台:
  //
  //  o  reshape 数组是 int 数组的数组，每一项用来填充给输入层的
  //
  //       caffe::Blob::Reshape(std::vector<int> const &);
  //
  //     函数，reshape 数组长度必须与 input 数组长度一样。
  //
  //     没有 reshape 选项时，不进行 reshape。有 reshape，但某个输出 i
  //     对应的的 reshape[i] 数组长度是 0 时，不对输入 i 进行 reshape。
  int LoadOther(OperatorGraphContext *context, toml::value const &conf);

 public:
  // 从缓冲区读取数据，运行推理，输出到缓冲区。
  //
  // NeuralNetwork 负责修改 output 的大小。当它与 output_size_ 不匹配时，
  // NnOperator 会返回错误。所以它不必对此一致性作出检查。
  //
  // 但当网络结构导致无此尺寸信息时不能进行推理，则可以取得 output_size()
  // 信息， 它在 Forward 时是健全的。
  int Forward(std::vector<BufferView> const &input,
              std::vector<BufferView> &output);
  int Forward(std::vector<BufferView> const &input,
              std::vector<NdArray> &output);

  std::vector<size_t> const &output_size() const { return output_size_; }
  std::vector<size_t> const &input_size() const { return input_size_; }
  bool output_ndarray() const { return output_ndarray_; }

 private:
  std::shared_ptr<Impl> impl_;
  std::vector<size_t> input_size_;
  std::vector<size_t> output_size_;
  bool output_ndarray_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_NN_H_
