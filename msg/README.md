#以pb目录下的所有proto文件为整体确定一个版本号   
#每次更该proto文件后需要同步更新一同目录下的Version文件里的版本号   

使用protobuffer 工具生成.cc和.h，然后执行以下命令，就会把所有proto生成一遍,生成文件在../precompiler下：   
```
$ ./gen.sh
```

# data_proto
**---**
### pb : 数据协议
+ calib_param.proto : CalibParam -> 标定参数
+ camera.proto : CameraFrame -> 获取的图像数据结构
+ camera_occlusion.proto : 摄像头遮挡结果数据结构
+ geometry.proto : 几何结构
+ imu.proto : imu数据结构
+ odometry.proto : 里程计数据结构
+ road_seg.proto : 路面分割信息数据结构
+ roadmarking.proto : 路面感知结果数据结构
+ scene.proto : 场景感知数据结构
+ vehicle.proto : 车辆感知结果数据结构
+ pedestrain.proto : 行人感知结果数据结构
+ tsr.proto : tsr感知结果数据结构
+ ihc.proto : ihc感知结果数据结构
+ cipv.proto : 融合关键车结果数据结构
+ fail_detection.proto : 失效检测
+ decision_and_planning.proto : 决策规划结果数据结构
+ fusion.proto : 1V1R融合输出数据结构
+ object_camera.proto : 视觉目标检测系统对外输出的数据结构
+ radar.proto : 毫米波雷达感知结果输入结构体
+ HafLocation.proto : 融合定位输出结构体
+ data_header.proto : 时间信息结构体
+ gnss.proto : 卫导信息结构体
+ avm_calib_param.proto： 环视标定参数设置结构体
+ freespacepoints.proto: freespace分割模型输出的可通行区域的边界点数据结构
+ freespace_obstacles.proto: freespace经过gridmap模块建图后输出的障碍物数据结构
+ apa_state.proto： apa系统状态管理模块数据结构
+ parking_mask.proto： 车位线clusters数据结构体
+ parking_object.proto:  检测目标2d/3d数据结构体


### 目标检测数据协议发布计划

V1.x版本

维持现有目标检测proto分类和定义方式不变，不更改同下游的协作方式,V1.x版本面向L0~L2。

V2.x版本

将vehicle,ped,ihc合并到一个obj，和mdc及mobileye做法一致,V2.x版本面向L2~L3。
