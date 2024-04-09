#!/bin/bash

export LD_LIBRARY_PATH=/usr/lib:/app/algo/avmcalib/lib:${LD_LIBRARY_PATH}
if [ ! -e /usr/lib/libdlr.so ]; then
  ln -s /usr/lib/python3.8/site-packages/libdlr.so /usr/lib/libdlr.so
fi

cd /app/algo/avmcalib

./bin/avm_calib_service \
--config_file="config/avm_calib.json" \
--topic_camera="camera30" \
--topic_avm_status="avm_calib_status" \
--topic_avm_ctrl="avm_calib_ctrl" \
--car_file="config/car_info.json" \
--layout_file="config/avm_layout.json" \
--intrins_file_path="config/" \
--save_exp_path="/calib/avm_calib/" \
--err_threshold=0.05 \
--loglevel=3 \
> /dev/null 2>&1 &
