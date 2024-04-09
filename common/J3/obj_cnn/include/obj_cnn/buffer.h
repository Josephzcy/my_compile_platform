// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_BUFFER_H_
#define INCLUDE_OBJ_CNN_BUFFER_H_

#include <cstddef>
#include <memory>
#include <vector>

namespace obj_cnn {

// BufferView is a IO interface of Buffer.
class BufferView {
 public:
  class Impl;

  /** Create a empty BufferView. */
  BufferView();

  explicit BufferView(std::shared_ptr<Impl> impl);

  // 从字节数组加载 BufferView，在当前实例不空的时候返回错误。
  int LoadBuffer(uint8_t *data, size_t n);

  /** Use a bytes array to create buffer, buffer own this array after
   * this, if possiabe.
   *
   * \return 0 if succeed.
   */
  int LoadBufferMove(uint8_t *data, size_t n);

  // Bytes count of this buffer.
  size_t bytes() const;

  // Bytes start point in CPU memory.
  uint8_t *Raw() const;

  // Clone of BufferView will share the prototype of underlying Buffer.
  BufferView Clone();

  std::shared_ptr<Impl> impl() const { return impl_; }

  bool IsEmpty() const { return impl_ == nullptr; }

  // BufferView should not be clone or move implicitly.
 private:
  std::shared_ptr<Impl> impl_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_BUFFER_H_
