#!/bin/bash

for i in 1 2 3 4 5 6 7 8
do
  ./linux_Release/bin/publish_video_client $i >/dev/null 2>&1 &
done
