// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_TYPE_H_
#define INCLUDE_OBJ_CNN_TYPE_H_

namespace obj_cnn {

// Mark a type not implicitly copyable. This is called "Move" because Rust
// marked all types moveable. But simple type "Copy" iff it can be implicitly
// copied.
struct Move {
  // Move can be default construted, so that the type derived it don't
  // have to initialize it.
  Move() {}

  Move(Move const &) = delete;
  Move &operator=(Move const &) = delete;

  Move(Move &&) = default;
  Move &operator=(Move &&) = default;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_TYPE_H_
