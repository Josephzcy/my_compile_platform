# stop postprocess
postprocess_pid=`ps -aux|grep "postprocess" |grep -v "grep"|awk '{print $2}'`
kill -9 $postprocess_pid
sleep 0.1

# stop havp_inference
havp_inference_pid=`ps -aux|grep "havp_inference" |grep -v "grep"|awk '{print $2}'`
kill -9 $havp_inference_pid
sleep 0.1

# stop apa_ultrasonic_parkingspace
apa_ultrasonic_parkingspace_pid=`ps -aux|grep "apa_ultrasonic_parkingspace" |grep -v "grep"|awk '{print $2}'`
kill -9 $apa_ultrasonic_parkingspace_pid
sleep 0.1

# stop sensor_fusion
sensor_fusion_pid=`ps -aux|grep "sensor_fusion" |grep -v "grep"|awk '{print $2}'`
kill -9 $sensor_fusion_pid
sleep 0.1

# stop apa_localization
apa_localization_pid=`ps -aux|grep "apa_localization" |grep -v "grep"|awk '{print $2}'`
kill -9 $apa_localization_pid
sleep 0.1

# stop bird_eye_view
bird_eye_view_pid=`ps -aux|grep "bird_eye_view" |grep -v "grep"|awk '{print $2}'`
kill -9 $bird_eye_view_pid
sleep 0.1

# stop planning
planning_dds_pid=`ps -aux|grep "planning" |grep -v "grep"|awk '{print $2}'`
kill -9 $planning_dds_pid
sleep 0.1

# stop vehicle_control_dds
vehicle_control_dds_pid=`ps -aux|grep "vehicle_control_dds" |grep -v "grep"|awk '{print $2}'`
kill -9 $vehicle_control_dds_pid
sleep 0.1

# stop apa_manager.sh
apa_manager_pid=`ps -aux|grep "apa_manager" |grep -v "grep"|awk '{print $2}'`
kill -9 $apa_manager_pid
sleep 0.1