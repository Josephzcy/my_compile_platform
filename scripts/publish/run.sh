# start run_apa_manager_dds.sh
./script/run_apa_manager_dds.sh >/dev/null 2>&1 &

# start run_apa_localization.sh
./script/run_apa_localization_t1q3.sh >/dev/null 2>&1 &

# start run_bev.sh
./script/run_bev.sh >/dev/null 2>&1 &

# start run_havp_inference.sh
./script/run_havp_inference.sh >/dev/null 2>&1 &

# start run_uss_ps.sh
./script/run_uss_ps.sh >/dev/null 2>&1 &

# start run_sensor_fusion.sh
./script/run_sensor_fusion.sh >/dev/null 2>&1 &

# start run_postprocess.sh
./script/run_postprocess.sh >/dev/null 2>&1 &

# start run_apa_planning_dds_x50.sh
#./script/run_apa_planning_dds_x50.sh >/dev/null 2>&1 &

# start run_vehicle_control_dds.sh
./script/run_vehicle_control_dds.sh >/dev/null 2>&1 &

# start run_vtr.sh
#bash ./script/run_vtr.sh >/dev/null 2>&1 &

# start run_ap_slam.sh
./script/run_ap_slam.sh >/dev/null 2>&1 &

# start run_imu_asensing.sh
./script/run_imu_asensing.sh >/dev/null 2>&1 &

# start run_parking_mod.sh
./script/run_parking_mod.sh >/dev/null 2>&1 &