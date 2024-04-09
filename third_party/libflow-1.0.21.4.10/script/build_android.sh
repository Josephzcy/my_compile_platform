#!/bin/bash

BuildType="Release"
if [ $# -eq 1 ]; then
  BuildType="$1"
fi

BuildDir="android"
mkdir -p ${BuildDir}
cd ${BuildDir}

cmake -DCMAKE_TOOLCHAIN_FILE="../script/android.toolchain.cmake" \
      -DCMAKE_BUILD_TYPE="${BuildType}" \
      -DANDROID_NATIVE_API_LEVEL="android-21" \
      -DANDROID_ABI="arm64-v8a" \
      -DANDROID_TOOLCHAIN_NAME="aarch64-linux-android-4.9" \
      ..

make -j $(nproc)

$ANDROID_TOOLCHAIN_ARM64/bin/aarch64-linux-android-strip bin/test1
$ANDROID_TOOLCHAIN_ARM64/bin/aarch64-linux-android-strip bin/test2
