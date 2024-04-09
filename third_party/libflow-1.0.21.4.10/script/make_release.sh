#!/bin/bash

set -e

if [ "_$1" = "_" ]; then
  echo "Usage: $0 version"
  exit 1
fi

if [ ! -f linux/src/libflow.a ]; then
  echo "Error: cannot find linux/src/libflow.a"
fi

if [ ! -f arm64/src/libflow.a ]; then
  echo "Error: cannot find arm64/src/libflow.a"
fi

Version="$1"
TopDir=`pwd`
LinuxDir="libflow-${Version}-linux-amd64"
AndroidDir="libflow-${Version}-android-arm64"

mkdir -p dist
cd dist

mkdir -p $LinuxDir/{include,lib}
cp ../src/flow.hpp $LinuxDir/include/
cp ../linux/src/libflow.a $LinuxDir/lib/
tar czf $LinuxDir.tar.gz $LinuxDir

mkdir -p $AndroidDir/{include,lib}
cp ../src/flow.hpp $AndroidDir/include/
cp ../arm64/src/libflow.a $AndroidDir/lib/
tar czf $AndroidDir.tar.gz $AndroidDir
