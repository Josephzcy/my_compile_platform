#!/bin/bash
Program=${0##*/}

show_usage() {
  echo "Usage: $Program [Options] <target> [build_type]"
  echo
  echo "Options:"
  echo "  -h, --help             Show help message and exit"
  echo
  echo "Target:"
  echo "  linux (default)"
  echo "  armv8-android"
  echo "  armv7-linux"
  echo "  armv8-linux"
  echo "  armv7-android"
  echo "  cv22"
  echo
  echo "Build type:"
  echo "  release (default)"
  echo "  debug"
  echo
  echo "Project:"
  echo "  linux-pc (default)"
  echo "  master"
  echo "  skoda"
  echo "  tang20"
  echo "  dev"
  echo
  echo "Para:"
  echo "  env (para from env)"
  echo "  gflags (para from gflasg)"
  echo
}

if [ $# -eq 0 ];then
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
    if [ "_$Target" = "_" ]; then
      Target="$1"
      shift
    elif [ "_$BuildType" = "_" ]; then
      case "${1,,}" in
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
    elif [ "_$Project" = "_" ]; then
        Project="$1"
        shift
    elif [ "_$Para" = "_" ]; then
        Para="$1"
        shift
    else
      echo "Error: too many positional arguments"
      exit 1
    fi
    ;;
  esac
done

if [ "_$Target" = "_" ]; then
    LibMode=linux
fi

if [ "_$BuildType" = "_" ]; then
    BuildType=Release
fi

if [ "_$Project" = "_" ]; then
    Project=dev
fi

if [ "_$Para" = "_" ]; then
    Para=gflags
fi

. "$(dirname "$0")/build_config.sh" ${Target} ${BuildType} ${Project} ${Para}
. "$(dirname "$0")/build_modules_config.sh" ${Project}

#
for var in ${module_names[@]};
do
    . "$(dirname "$0")/build_process.sh" $var
done
