#!/bin/bash

BuildType="Release"
if [ $# -eq 1 ]; then
  BuildType="$1"
fi

BuildDir="zynq"
mkdir -p ${BuildDir}
cd ${BuildDir}

cmake -DCMAKE_BUILD_TYPE="$BuildType" \
      -DCMAKE_SYSTEM_NAME=Linux \
      -DCMAKE_C_COMPILER=/usr/bin/arm-linux-gnueabihf-gcc-4.9 \
      -DCMAKE_CXX_COMPILER=/usr/bin/arm-linux-gnueabihf-g++-4.9 \
      -DCMAKE_AR=/usr/bin/arm-linux-gnueabihf-ar \
      -DPLATFORM=arm \
      ..

make -j $(nproc)

strip bin/test1
strip bin/test2
