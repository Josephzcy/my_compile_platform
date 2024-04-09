/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 * Author: Nicholas Zhang
 * Version: 1.0.0
 * Time: 2022-10-17 10:50:23
 *****************************************************************************/

#ifndef _BAZEL_COMPONENT_H_
#define _BAZEL_COMPONENT_H_

#include <iostream>


namespace minieye{
namespace bazel{
class BazelComponent{
public:
  BazelComponent();
  bool init();
private:
  int status;
  };
}
}

#endif
