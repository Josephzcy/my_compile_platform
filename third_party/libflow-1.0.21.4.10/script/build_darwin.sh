#!/bin/bash

BuildType="Release"
if [ $# -eq 1 ]; then
  BuildType="$1"
fi

BuildDir="darwin"
mkdir -p ${BuildDir}
cd ${BuildDir}

cmake -DCMAKE_BUILD_TYPE="$BuildType" ..
make -j $(sysctl -n hw.ncpu)
