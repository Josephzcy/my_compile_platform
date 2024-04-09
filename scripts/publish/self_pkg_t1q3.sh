#!/bin/bash
Program=${0##*/}

RUN_USER=$1
RUN_KEY=$2

if type curl >/dev/null 2>&1 ;
  then
    echo 'start pkg .....'
  else
    echo 'please install curl'
    sudo apt install curl
fi

TOKEN=b7a323fa41d22e4f5929b4e302e75b
BRANCH_NAME=master_havp_t1q3
if [ "$RUN_USER" == "" ]; then
  curl -X POST --fail -F token=${TOKEN} -F ref=${BRANCH_NAME} \
          https://git.minieye.tech/api/v4/projects/3256/trigger/pipeline >/dev/null 2>&1
else
  curl -X POST --fail -F token=${TOKEN} -F ref=${BRANCH_NAME} -F "variables[${RUN_USER}]=${RUN_KEY}" \
          https://git.minieye.tech/api/v4/projects/3256/trigger/pipeline >/dev/null 2>&1
fi

echo 'PKG Finished Please check in the aiplorer'
