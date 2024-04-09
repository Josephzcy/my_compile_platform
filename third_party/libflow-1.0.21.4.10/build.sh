#!/bin/bash

set -e

Program=${0##*/}
Version="0.1.0"

show_usage() {
  echo "Usage: $Program [Options] <platform> [build_type] [target]"
  echo
  echo "Options:"
  echo "  -h, --help           Show help message and exit"
  echo "  -v, --version        Show version message and exit"
  echo
  echo "Platform:"
  echo "  windows-x86"
  echo "  windows-x86_64"
  echo "  mac"
  echo "  linux-x86"
  echo "  linux-x86_64"
  echo "  linux-arm"
  echo "  linux-arm64"
  echo "  android-arm"
  echo "  android-arm64"
  echo
  echo "Build type:"
  echo "  release (default)"
  echo "  debug"
  echo
  echo "Target:"
  echo "  all (default)"
  echo
}

show_version() {
  echo "$Program $Version"
}

check_android_ndk() {
  REQUIRED_NDK_VERSION=$1
  Hint="Hint: install Android NDK $REQUIRED_NDK_VERSION"

  if [ "_$ANDROID_NDK" = "_" ]; then
    echo "Error: \$ANDROID_NDK is missing"
    echo $Hint
    exit 1
  fi

  NDK_NAME=$(basename $ANDROID_NDK)
  NDK_VERSION=${NDK_NAME##*-}
  if [ "_$NDK_VERSION" != "_$REQUIRED_NDK_VERSION" ]; then
      echo "Error: invalid NDK version '$NDK_VERSION'"
      echo $Hint
      exit 1
  fi
}

set_env_for_android_arm64() {
  CMakeFlags=$(cat <<END
-DCMAKE_SYSTEM_NAME=Android \
-DCMAKE_MAKE_PROGRAM=/usr/bin/make \
-DCMAKE_TOOLCHAIN_FILE=script/m4.android.toolchain.cmake \
-DANDROID_NDK=$ANDROID_NDK \
-DANDROID_NATIVE_API_LEVEL=21 \
-DANDROID_ABI=arm64-v8a \
-DANDROID_TOOLCHAIN_NAME=aarch64-linux-android-4.9
END
)
}

set_env_for_android_arm() {
  CMakeFlags=$(cat <<END
-DCMAKE_SYSTEM_NAME=Android \
-DCMAKE_MAKE_PROGRAM=/usr/bin/make \
-DCMAKE_TOOLCHAIN_FILE=script/c1disp.android.toolchain.cmake \
-DANDROID_NDK=$ANDROID_NDK \
-DANDROID_PLATFORM=android-22 \
-DANDROID_NATIVE_API_LEVEL=22 \
-DANDROID_ABI=armeabi-v7a \
-DANDROID_ARM_NEON=ON \
-DANDROID_TOOLCHAIN_NAME=clang
END
)
}

set_env_for_linux_arm() {
  GCC_HOME=/usr/bin
  CMakeFlags=$(cat <<END
-DCMAKE_SYSTEM_NAME=Linux \
-DCMAKE_C_COMPILER=$GCC_HOME/arm-linux-gnueabihf-gcc-4.9 \
-DCMAKE_CXX_COMPILER=$GCC_HOME/arm-linux-gnueabihf-g++-4.9 \
-DCMAKE_AR=$GCC_HOME/arm-linux-gnueabihf-ar
END
)
}

while [[ $# -gt 0 ]]
do
  case "$1" in
    -h|--help)
      show_usage
      exit
      ;;
    -v|--version)
      show_version
      exit
      ;;
    -*)
      echo "Error: unknown option: $1"
      exit 1
      ;;
    *)
      if [ "_$Platform" = "_" ]; then
        Platform="$1"
        shift
      elif [ "_$BuildType" = "_" ]; then
        LowerCase=$(echo $1 | awk '{print tolower($0)}')
        case "$LowerCase" in
          release)
            BuildType=Release
            ;;
          debug)
            BuildType=Debug
            ;;
          *)
            echo "Error: invalid build type"
            exit 1
            ;;
        esac
        shift
      elif [ "_$MakeTarget" = "_" ]; then
        MakeTarget="$1"
        shift
      else
        echo "Error: too many positional arguments"
        exit 1
      fi
      ;;
  esac
done

if [ "_$Platform" = "_" ]; then
  show_usage
  exit 1
fi

case "$Platform" in
  windows*)
    echo "Please refer to the Visual Studio project under ./windows/StaticLib/"
    exit 1
    ;;
  mac)
    DirPrefix=mac
    ;;
  linux-x86)
    echo "Error: 32-bit x86 Linux system is no longer supported"
    exit 1
    ;;
  linux-x86_64)
    DirPrefix=linux-x86_64
    #CMakeFlags="-DCMAKE_CXX_FLAGS_RELEASE=-L/home/zhangqi/src/main/flow-dylib/install/linux/lib $CMakeFlags"
    ;;
  linux-arm)
    DirPrefix=linux-arm
    set_env_for_linux_arm
    #CMakeFlags="-DCMAKE_CXX_FLAGS_RELEASE=-L/home/zhangqi/src/main/flow-dylib/install/x1/lib $CMakeFlags"
    ;;
  android-arm)
    check_android_ndk r19c
    DirPrefix=android-arm
    set_env_for_android_arm
    ;;
  android-arm64)
    check_android_ndk r11c
    DirPrefix=android-arm64
    set_env_for_android_arm64
    #CMakeFlags="-DCMAKE_CXX_FLAGS_RELEASE=-L/home/zhangqi/src/main/flow-dylib/install/m4/lib $CMakeFlags"
    ;;
  *)
    echo "Error: invalid platform"
    exit 1
esac

CMakeFlags="-DMY_BUILD_PLATFORM=$Platform $CMakeFlags"
export Platform

# determine build type
if [ "_$BuildType" = "_" ]; then
  BuildType=Release
fi

echo "Build type: $BuildType"
export BuildType

# make the build dir (if not exist)
BuildDir="${DirPrefix}_${BuildType}"
echo "Build dir: $BuildDir"
mkdir -p "$BuildDir"
cd "$BuildDir"

# build
CMakeFlags="-DCMAKE_BUILD_TYPE=$BuildType $CMakeFlags"
echo "Build flags: $CMakeFlags"

cmake $CMakeFlags -DBUILD_SHARED_LIBS=ON ..

OS=$(uname)
if [ "_$OS" = "_Linux" ]; then
  Cores=$(nproc)
elif [ "_$OS" = "_Darwin" ]; then
  Cores=$(sysctl -n hw.ncpu)
else
  echo "Warning: unsupported OS: $OS"
  Cores=4
fi

make -j $Cores $MakeTarget
