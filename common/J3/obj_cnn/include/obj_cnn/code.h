// Copyright 2023 MINIEYE

#ifndef INCLUDE_OBJ_CNN_CODE_H_
#define INCLUDE_OBJ_CNN_CODE_H_

namespace obj_cnn {

enum Code {
  CODE_OK = 0,
  CODE_ERROR,  // Unexpected error.
  CODE_TYPE_ERROR,
  CODE_INPUT_LENGTH_ERROR,
  CODE_OUTPUT_LENGTH_ERROR,
  CODE_OUT_OF_RANGE,
  CODE_NULL,
  CODE_NODE_NAME,
  CODE_NOT_CLONEABLE,
  CODE_INVALID_INPUT,
  CODE_LIBYUV,  // libyuv report error
  CODE_TODO,
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_CODE_H_
