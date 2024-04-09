#!/bin/bash
#set -x
set -e
set -o pipefail

RED_COLOR='\E[1;31m'   #红
GREEN_COLOR='\E[1;32m' #绿
YELOW_COLOR='\E[1;33m' #黄
BLUE_COLOR='\E[1;34m'  #蓝
RES='\E[0m'

script_sh=${0##*/}

show_usage() {
  echo "Usage: $script_sh [Options]"
  echo "Or"
  echo "Usage: $script_sh <Platform> <Build_type> <Target>"
  echo
  echo "Options:"
  echo "  -h|--help   Show help message and exit"
  echo
  echo "Platform:"
  echo "  linux"
  echo "  tda4"
  echo "  J3"  
  echo "  armv8-linux"
  echo "  armv8-android"
  echo "  ..."
  echo
  echo "Build type:"
  echo "  debug"
  echo "  release"
  echo
  echo "Target:"
  echo "  //examples/hello-world-bazel:hello-world"
  echo "  //examples/sayhello-bazel/main:hello-world"
  echo "  //aipilot/modules/apa_manager:apa_manager"
  echo "  ..."
  echo
}

#echo "~~~~~ $#  ~~~~~"
if [[ $# -eq 0 ||  $# -lt 3 ]];then
    show_usage
    exit 1
fi

while [[ $# -gt 0 ]]
do
  case "$1" in
    -h|--help)
      show_usage
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
      case "${1,,}" in
        release|debug)
          BuildType=$1
          ;;
        *)
          echo "Error: invalid build type"
          exit 1
          ;;
      esac
      shift
    elif [ "_$Target" = "_" ]; then
        Target="$1"
        shift
    else
      echo "Error: too many arguments"
      exit 1
    fi
    ;;
  esac
done

echo -e "\${Platform} \${BuildType} \${Target}"
echo  ${Platform} ${BuildType} ${Target}

ExtraConfig="--cxxopt=-std=c++11"
if [ "$Platform" = "tda4" ];then 
    ExtraConfig="${ExtraConfig} --config=tda4 --copt=-DWITH_TDA4"
elif [ "$Platform" = "J3" ];then 
    ExtraConfig="${ExtraConfig} --config=J3 --copt=-DWITH_J3"    
elif [ "$Platform" = "linux" ];then 
    ExtraConfig="${ExtraConfig} --copt=-DWITH_LINUX_PC"
else
    echo "unkonwn platform"
    exit 1  
fi

echo 'PLATFORM="'${Platform}'"' >  .repo.bzl

if [ "$BuildType" = "release" ];then
    ExtraConfig="${ExtraConfig} -c opt --copt=-O2 --copt=-DNDEBUG --strip=always --linkopt=-Wl,--strip-all"
elif [ "$BuildType" = "debug" ];then
    ExtraConfig="${ExtraConfig} --copt=-g -c dbg --strip=never"
else
    echo "not supported build type"
    exit 1 
fi

#echo -e "${BLUE_COLOR} bazel build \$ExtraConfig \${Target} ${RES}"
echo -e "${GREEN_COLOR} CMD: [bazel build $ExtraConfig ${Target}] ${RES}"
#bazel build -s $ExtraConfig ${Target} --explain="log.txt" --verbose_explanations=true --worker_verbose=true   

bazel build $ExtraConfig ${Target}
rm .repo.bzl
