// Copyright 2023 MINIEYE

#ifndef INCLUDE_OBJ_CNN_CODE_TO_STR_H_
#define INCLUDE_OBJ_CNN_CODE_TO_STR_H_

#include "obj_cnn/interface.h"
#include "obj_cnn/code.h"

namespace obj_cnn {

template <>
char const *ToStr<Code>(const Code &code);

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_CODE_TO_STR_H_
