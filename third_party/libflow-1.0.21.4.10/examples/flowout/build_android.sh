#!/bin/bash

set -e

if [ ! -e "$GOROOT/bin/go" ]; then
  echo "Error: invalid GOROOT"
  exit 1
fi

export GOOS=linux
export GOARCH=arm64

echo "Generating build_time.go"
Timestamp=`date +%Y%m%d%H%M%S`
echo -e "package main\nconst BuildTime = \"$Timestamp\"" > build_time.go

BinDir=.
Target=flowout-android-${GOARCH}
echo "Building ${Target}"
$GOROOT/bin/go build -v \
    -o ${BinDir}/${Target} \
    flowout.go \
    wsclient.go \
    utils.go \
    decode.go \
    build_time.go

$ANDROID_TOOLCHAIN_ARM64/bin/aarch64-linux-android-strip ${BinDir}/$Target
