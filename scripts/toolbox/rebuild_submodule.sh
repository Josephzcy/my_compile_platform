#!/bin/bash
set -x
pushd .

cat /dev/null > .gitmodules
git add .gitmodules

rm -rf .git/modules
sed -i '/submodule/,$d' .git/config

rm -rf aipilot/modules/*

rm -rf aipilot/services/wheel_radius_calibration

#rm -rf tools/apa_odom_viewer
rm -rf tools/ddsflow


#git rm --cached aipilot/modules/dfg_ultrasonic_parkingspot
#git rm --cached aipilot/modules/dfg_apa_transparent
git rm --cached aipilot/modules/apa_manager
git rm --cached aipilot/modules/apa_localization
git rm --cached aipilot/modules/havp_inference
git rm --cached aipilot/modules/havp_parkingspace_postprocess
git rm --cached aipilot/modules/apa_planning
git rm --cached aipilot/modules/vehicle_control_dds

git rm --cached aipilot/services/wheel_radius_calibration

#git rm --cached tools/apa_odom_viewer
git rm --cached tools/ddsflow

cd aipilot/modules
#git submodule add git@git.minieye.tech:huzihao/dfg_ultrasonic_parkingspot.git
#git submodule add -b feature_tda4 git@git.minieye.tech:wangnianbing/dfg_apa_transparent.git
git submodule add -b apa_manager_l_byd git@git.minieye.tech:huangwei0527/apa_manager.git
git submodule add -b apa_dev git@git.minieye.tech:xujian/apa_localization.git
git submodule add git@git.minieye.tech:wujiang/havp_inference.git
git submodule add -b master_apal_byd git@git.minieye.tech:FusionGroup/havp_parkingspace_postprocess.git
git submodule add -b dev_havp_planning git@git.minieye.tech:fanyu/apa_planning.git
git submodule add -b dev_byd git@git.minieye.tech:zhangpeng/vehicle_control_dds.git

cd ../services
git submodule add git@git.minieye.tech:zhangchengyu/wheel_radius_calibration.git


cd ../../tools
#git submodule add git@git.minieye.tech:xuezhaohan/apa_odom_viewer.git 
git submodule add -b ddsflow_dev git@git.minieye.tech:zhangchengyu/ddsflow.git

popd
