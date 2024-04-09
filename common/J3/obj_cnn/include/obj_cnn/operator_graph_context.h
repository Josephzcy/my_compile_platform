// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATOR_GRAPH_CONTEXT_H_
#define INCLUDE_OBJ_CNN_OPERATOR_GRAPH_CONTEXT_H_

#include <cstdio>
#include <ostream>
#include <string>
#include <vector>
#include <memory>

namespace obj_cnn {

class OperatorGraphContext {
 public:
  class Impl;

  OperatorGraphContext();

  std::shared_ptr<Impl> impl() { return impl_; }

 private:
  std::shared_ptr<Impl> impl_;
};
}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATOR_GRAPH_CONTEXT_H_
