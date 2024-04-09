#!/bin/bash

BuildType="Release"
if [ $# -eq 1 ]; then
  BuildType="$1"
fi

BuildDir="linux"
mkdir -p ${BuildDir}
cd ${BuildDir}

cmake -DCMAKE_BUILD_TYPE="$BuildType" \
      -DPLATFORM=amd64 \
      ..

make -j $(nproc)

strip bin/test1
strip bin/test2
