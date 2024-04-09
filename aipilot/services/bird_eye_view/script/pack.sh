#!/bin/bash
FINAL_PATH=${1:-a_default}
IP=/home/mini/wujiang/others/havp_transfer/$FINAL_PATH

if [ ! -d ${IP}/bin ]; then
    mkdir -p ${IP}/bin
fi
if [ ! -d ${IP}/flag ]; then
    mkdir -p ${IP}/flag
fi
if [ ! -d ${IP}/script ]; then
    mkdir -p ${IP}/script
fi

scp build/tda4/bin/bird_eye_view ${IP}/bin
scp flag/bev.flag ${IP}/flag
scp script/run_bev.sh ${IP}/script

echo "pack completed"
nautilus ${IP}
