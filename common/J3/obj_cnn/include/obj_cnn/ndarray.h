// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_NDARRAY_H_
#define INCLUDE_OBJ_CNN_NDARRAY_H_

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "obj_cnn/interface.h"

namespace obj_cnn {

enum class NdArrayDataType { Int8, Uint8, Int16, Uint16, Int32, Uint32, Int64, Uint64, Float };

template<>
const char *ToStr<NdArrayDataType>(const NdArrayDataType &);

template<>
Code TryFromStr<NdArrayDataType>(const char *text, NdArrayDataType *);

struct DimHead {
  explicit DimHead(size_t size_arg) : size(size_arg) {}

  size_t size;
};

struct DimTail {
  explicit DimTail(size_t size_arg) : size(size_arg) {}

  size_t size;
};

/// Dim 是 NdArray 的一维的长度。
///
/// NdArray 允许存在 padding，也就是逻辑上有效的数据之外可以有无效的垫料。size 是有效长度，head 和 tail 分别是前后的
/// 无效垫料。
struct Dim {
  explicit Dim(size_t size) : size_(size), head_(0), tail_(0) {}

  Dim(size_t size, DimHead head, DimTail tail) : size_(size), head_(head.size), tail_(tail.size) {}

  size_t full_size() const { return size_ + head_ + tail_; }
  size_t size() const { return size_; }
  size_t head() const { return head_; }
  size_t tail() const { return tail_; }

 private:
  size_t size_;
  size_t head_;
  size_t tail_;
};

// Dims 是对 Dim 的数组的包装，表示一个 NdArray 的形状，它额外保存了一些尺寸讯息，可以用在在 NdArray 中定位。
//
// 这是一个不可变的数据结构，如果你想依靠一个 Dims 为模板得到另一个 Dims，可以使用 TakeDims 销毁包装，再重新包装。
class Dims {
 public:
  Dims() : full_size_(0), total_avaliable_size_() {}

  explicit Dims(std::vector<Dim> dims)
      : dims_(std::move(dims)), full_size_(ComputeTotalSize()), total_avaliable_size_(ComputeTotalAvaliableSize()) {}

  // 访问内部的 Dim 的数组。
  const std::vector<Dim> &dims() const { return dims_; }

  // 销毁包装，得到内部的 Dim 数组。
  std::vector<Dim> &&TakeDims() && { return std::move(dims_); }

  // 所有维度数据，包含 padding 的总计个数。
  size_t total_size() const { return full_size_; }

  // dims[i] 这一维取偏移 indices[i]，得到一个元素举例开始位置的偏移。
  size_t offset(const std::vector<size_t> &indices) const;

  // 所有维度数据，不好好 padding 的总计个数。
  size_t total_avaliable_size() const { return total_avaliable_size_; }

  // 维度的个数。
  size_t size() const { return dims_.size(); }

  // 访问第 n（从 0 开始）个维度的尺寸。
  const Dim &operator[](size_t n) const { return dims_[n]; }

  // 支持不可变的迭代 Dim。
  using const_iterator = std::vector<Dim>::const_iterator;

  const_iterator begin() const { return dims_.begin(); }
  const_iterator end() const { return dims_.end(); }

 private:
  std::vector<Dim> dims_;
  size_t full_size_;
  size_t total_avaliable_size_;

  size_t ComputeTotalSize() const;
  size_t ComputeTotalAvaliableSize() const;
};

class NdArray {
 public:
  class Impl;

  NdArray();
  explicit NdArray(std::shared_ptr<Impl> impl);

  int Load(const Dims &dims, NdArrayDataType type, uint8_t *data, bool quantized, float scale);

  uint8_t *data() const;
  uint8_t *ptr(const std::vector<size_t> &indices) const;

  /// Number of items in this array.
  size_t full_size() const;

  /// Number of bytes in this array.
  size_t full_bytes() const;

  /// This is a adaptor for operator-nn. BufferView contains bytes() and operator-nn need this to check if result size
  /// match.
  ///
  /// This returns full_bytes() now, we might change size check out Nn operator to remove this.
  size_t bytes() const { return full_bytes(); }

  /// Size of one item in bytes.
  size_t item_bytes() const;

  const Dims &dims() const;

  bool quantized() const;
  float scale() const;

  NdArrayDataType type() const;

 private:
  std::shared_ptr<Impl> impl_;
};

class Array4d {
 public:
  Array4d(const Dim &dim_n, const Dim &dim_c, const Dim &dim_h, const Dim &dim_w, NdArrayDataType type, uint8_t *data,
          bool quantized, float scale);

  const Dim &dim_n() const;
  const Dim &dim_c() const;
  const Dim &dim_h() const;
  const Dim &dim_w() const;

  NdArrayDataType type() const;

  uint8_t *data() const;

  bool quantized() const;

  float scale() const;

  size_t bytes() const;

  uint8_t *ptr(size_t n, size_t c, size_t h, size_t w) const;

 private:
  NdArray array_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_NDARRAY_H_
