#pragma once

/*algo 31~26（6bit） component 25~20（6bit） function 19~10（10bit）
 errcode 9~0（10bit）*/

namespace minieye {

/***********Algo Module ************/

#define ALGO_MODULE_CAMERA_LANE             0x01 
#define ALGO_MODULE_CAMERA_OBJECT           0x02 

#define ALGO_MODULE_FUSION                  0x07 
#define ALGO_MODULE_PREDICTION              0x0A
#define ALGO_MODULE_PLANING                 0x0C

#define ALGO_MODULE_LOACTION                0x0D
#define ALGO_MODULE_PARKING                 0x0F

#define ALGO_MODULE_SOC_HARDWARE            0x10
#define ALGO_MODULE_SOC_PHM                 0x20

#define ALGO_MODULE_APA_LOCATION            0x11
#define ALGO_MODULE_APA_AVM                 0x12
#define ALGO_MODULE_APA_PARKSPACE_POSTPRO   0x13
#define ALGO_MODULE_APA_INFERENCE           0x14
#define ALGO_MODULE_APA_MANAGER             0x15
#define ALGO_MODULE_APA_BEV                 0x16
#define ALGO_MODULE_APA_CONTROL             0x17
#define ALGO_MODULE_APA_PLANNING            0x18

#define ALGO_MODULE_AVM_PARKING             0x30

/***********Component List************/

/*Camera_Lane*/
#define CLANE_INPUT              0x05
#define CLANE_PROCEDURE          0x06

/*Camera_Object*/
#define COBJECT_INPUT            0x01
#define COBJECT_SYSTEM           0x02
#define COBJECT_PROCEDURE        0x03
#define COBJECT_OUTPUT           0x04

/*fusion*/
#define CFUSION_INPUT            0x01
#define CFUSION_SYSTEM           0x02
#define CFUSION_PROCEDURE        0x03
#define CFUSION_OUTPUT           0x04

/*Planing*/
#define CPLANING_INPUT           0x01
#define CPLANING_SYSTEM          0x02
#define CPLANING_PROCEDURE       0x03
#define CPLANING_OUTPUT          0x04

/*soc*/
#define CSOC_FRONTCAM            0x01
#define CSOC_REARCAM             0x02
#define CSOC_AVMFRONT            0x03
#define CSOC_AVMREARCAM          0x04
#define CSOC_AVMLEFTCAM          0x05
#define CSOC_AVMRIGHTCAM         0x06
#define CSOC_IMU                 0x07
#define CSOC_CPU                 0x08
#define CSOC_PHY                 0x09
#define CSOC_FRADAR              0x10
#define CSOC_FRONTRIGHTRADAR     0x11
#define CSOC_FRONTLEFTRADAR      0x12
#define CSOC_REARRIGHTRADAR      0x13
#define CSOC_REARLEFTRADAR       0x14
#define CSOC_PRIVATECAN          0x15
#define CSOC_MCU_R5              0x16
#define CSOC_DSP_C66             0x17
#define CSOC_DSP_C71             0x18
#define CSOC_CHASSIS             0x19
#define CSOC_STATEMAN            0x20
#define CSOC_FRONTCAMSHELTER     0x31
#define CSOC_REARCAMSHELTER      0x32
#define CSOC_LVDS                0x33
/*phm*/
#define CPHM_ALGOID              0x01


/*APA*/
#define CAPA_INPUT               0x1
#define CAPA_SYSTEM              0x2
#define CAPA_PROCEDURE           0x3
#define CAPA_OUTPUT              0x4

/*APA_PLANNING*/
#define CAVM_INPUT               0x1  
#define CAVM_SYSTEM              0x2
#define CAVM_FILES               0x3
#define CAVM_OPENGLES            0x4
#define CAVM_OUTPUT              0x5

/***********Function List ************/

/*Camera_Lane*/
#define FLANE_CAMERAS             0x01
#define FLANE_CHASIS_SIGNALS      0x02
#define FLANE_IMU                 0x03
#define FLANE_CALIBRATION         0x04
#define FLANE_CNN_ACC             0x01
#define FLANE_PITCH_EST           0x02
#define FLANE_PERFILING           0x03

/*Camera_Object*/
#define FOBJECT_CAMERAS             0x01
#define FOBJECT_CHASIS_SIGNALS      0x02
#define FOBJECT_IMU                 0x03
#define FOBJECT_ROAD_PERCEPTION     0x04
#define FOBJECT_CALIBRATION         0x05

#define FOBJECT_RESOURCE            0x01
#define FOBJECT_PERFILING           0x02

#define FOBJECT_OBJECT_DETECTION          0x01
#define FOBJECT_CIPV_TRACK                0x02
#define FOBJECT_CAMERA_BLOCK              0x03
#define FOBJECT_VEHICLE_SELECTOR          0x04
#define FOBJECT_VEHICLE_REID              0x05
#define FOBJECT_VEHICLE_REG_CLS           0x06
#define FOBJECT_VEHICLE_SMOOTH            0x07
#define FOBJECT_VEHICLE_COMBINATION       0x08
#define FOBJECT_VEHICLE_DIST_MEASURE      0x09
#define FOBJECT_PED_SELECTOR              0x0A
#define FOBJECT_PED_REID                  0x0B
#define FOBJECT_PED_CLS                   0x0C
#define FOBJECT_PED_SMOOTH                0x0D
#define FOBJECT_PED_COMBINATION           0x0E
#define FOBJECT_PED_MEASURE               0x0F
#define FOBJECT_MULTI_VEHICLE_ASSOCIATION 0x10
#define FOBJECT_VEHICLE_TRACE_PREDICTION  0x11
#define FOBJECT_VEHICLE_TRACE_UPDATE      0x12
#define FOBJECT_CIPV                      0x13
#define FOBJECT_VEHICLE_BOX3D             0x14
#define FOBJECT_VEHICLE_WARNING           0x15
#define FOBJECT_MULTI_PED_ASSOCIATION     0x16
#define FOBJECT_PED_WARNING               0x17

#define FOBJECT_LIBFLOW                   0x01
#define FOBJECT_DDS                       0x02

/*fusion*/
#define FFUSION_NO_FRONT_CAMERA_VEHICLE_RES      0x01
#define FFUSION_NO_REAR_CAMERA_VEHICLE_RES       0x02
#define FFUSION_NO_FRONT_CAMERA_LANE_RES         0x03
#define FFUSION_NO_REAR_CAMERA_LANE_RES          0x04
#define FFUSION_NO_FRONT_RADAR                   0x05
#define FFUSION_NO_CORNER_RADAR                  0x06
#define FFUSION_NO_VEHCILE_SIGNAL                0x07

#define FFUSION_RESOURCE                         0x01
#define FFUSION_PERFILING                        0x02

#define FFUSION_FILTER_ERROR                     0x01
#define FFUSION_ASSOCIATION_BLOCK                0x02

#define FFUSION_LIBFLOW                          0x01
#define FFUSION_DDS                              0x02

/*Planing*/
#define FPlaning_ENOFRONTLANE                    0x01
#define FPlaning_ENOFUSION                       0x02
#define FPlaning_ENOVEHICLESINGAL                0x03
#define FPlaning_EFRONTLANEPBPARSE               0x04
#define FPlaning_EFUSIONPBPARSE                  0x05
#define FPlaning_EVEHICLESINGALPBPARSE           0x06

#define FPlaning_EMEMLEAK                        0x01
#define FPlaning_ECPULOW                         0x02
#define FPlaning_ECPUHIGH                        0x03
#define FPlaning_EPOSTPTIMEOUT                   0x04

#define FPLANING_DNP_COMMON                      0x01
#define FPLANING_DNP_EGO_MOTION_ESTIMATION       0x02
#define FPLANING_DNP_OBJECT_TRAIL_PROC           0x03
#define FPLANING_DNP_VISION_PLAUSIBILITY         0x04
#define FPLANING_DNP_STEERING_PATH_PROC          0x05
#define FPLANING_DNP_DECISION                    0x06
#define FPLANING_DNP_LATERAL_PLANNER             0x07
#define FPLANING_DNP_LONGITUDINAL_PLANNER        0x08
#define FPLANING_DNP_SIDE_FEATURE                0x09
#define FPLANING_DNP_EHR                         0x0A

#define FPLANING_LIBFLOW                         0x01
#define FPlaning_DDS                             0x02

/*soc hardware */
#define FSOC_DATA        0x1
#define FSOC_INIT        0x2
#define FSOC_DTC         0x2
#define FSOC_CABLE       0x3
#define FSOC_SERDES      0x4
#define FSOC_SENSOR      0x5

#define FSOC_THERMAL      0x1
#define FSOC_PHYLINK      0x1
#define FSOC_PHYCABLE     0x2

/*PHM hardware */

#define FPHM_TYPE_ALIVE       0x1
#define FPHM_TYPE_DEADLINE    0x2
#define FPHM_TYPE_LOCAL       0x3
#define FPHM_TYPE_WATCHDOG    0x4
#define FPHM_TYPE_SUPERVISE   0x5

/*apa*/
#define FAPA_MEMORY_LEAK                        0x01
#define FAPA_CPU_OCCUPATION_TOO_HIGH            0x02
#define FAPA_GPU_OCCUPATION_TOO_HIGH            0x03
#define FAPA_RUN_TIMEOUT                        0x04

#define FAPA_LIBFLOW_ERROR                      0x01
#define FAPA_DDS_ERROR                          0x02

#define FAPA_VEHICLE_SIGNAL_TIMEOUT             0x01
#define FAPA_SUB_VEHICLE_SIG_ERROR              0x02                                                  
#define FAPA_VEHICLE_SIG_DATA_ERROR             0x03
#define FAPA_IMU_DATA_TIME_OUT                  0x04
#define FAPA_SUB_IMU_DATA_ERROR                 0x05
#define FAPA_IMU_DATA_ERROR                     0x06
#define FAPA_IMU_PROC                           0x01
#define FAPA_ODOM_PROC                          0x02
#define FAPA_IMU_INIT_ERRROR                    0x03
#define FAPA_LOST_LOCALIZATION                  0x03

#define FAPA_CAMERA_NOT_CALIBRATED              0x01
#define FAPA_LOAD_CONFIG                        0x02
#define FAPA_1CH_CAMERA_DATA_LOST               0x03
#define FAPA_2CH_CAMERA_DATA_LOST               0x04
#define FAPA_3CH_CAMERA_DATA_LOST               0x05
#define FAPA_4CH_CAMERA_DATA_LOST               0x06
#define FAPA_1CH_CAMERA_CALIBRATED_FAILED       0x01
#define FAPA_2CH_CAMERA_CALIBRATED_FAILED       0x02
#define FAPA_3CH_CAMERA_CALIBRATED_FAILED       0x03
#define FAPA_4CH_CAMERA_CALIBRATED_FAILED       0x04
#define FAPA_1CH_NO_FOUND_CONNER                0x05
#define FAPA_2CH_NO_FOUND_CONNER                0x06
#define FAPA_3CH_NO_FOUND_CONNER                0x07
#define FAPA_4CH_NO_FOUND_CONNER                0x08
#define FAPA_1CH_SAVE_CALIBRATED_RESULT         0x01
#define FAPA_2CH_SAVE_CALIBRATED_RESULT         0x02
#define FAPA_3CH_SAVE_CALIBRATED_RESULT         0x03
#define FAPA_4CH_SAVE_CALIBRATED_RESULT         0x04

#define FAPA_SIGHT_INPUT_ABNORMAL               0x01
#define FAPA_SIGHT_SUB_ERROR                    0x02
#define FAPA_SIGHT_INPUT_TIMEOUT                0x03
#define FAPA_RADAR_INPUT_ABNORMAL               0x04
#define FAPA_RADAR_SUB_ERROR                    0x05
#define FAPA_RADAR_INPUT_TIMEOUT                0x06
#define FAPA_VEHICLE_SIGNAL_INPUT_ABNORMAL      0x07
#define FAPA_VEHICLE_SIGNAL_SUB_ERROR           0x08
#define FAPA_VEHICLE_SIGNAL_INPUT_TIMEOUT       0x09
#define FAPA_CORNERPTS_TIMEOUT                  0x01
#define FAPA_PSDETECT_TIMEOUT                   0x02
#define FAPA_PSTRACK_TIMEOUT                    0x03
#define FAPA_FUSION_TIMEOUT                     0x04
#define FAPA_LOSE_PARKINGSPACE                  0x03

#define FAPA_NO_BEV_INPUT                       0x01
#define FAPA_BEV_INPUT_ABNORMAL                 0x02
#define FAPA_TAD4_ACC_ERROR                     0x01
#define FAPA_CNN_ERROR                          0x02
#define FAPA_MASK_GENERATE_ERROR                0x03
#define FAPA_FPS_TOO_LOW                        0x04

#define FAPA_CAMERA_DATA_ERROR                  0x01
#define FAPA_ODOMETRY_DATA_ERROR                0x02
#define FAPA_FREESPACE_INFERENCE_ERROR          0x01
#define FAPA_GRIDMAP_BUILD_ERROR                0x02
#define FAPA_OBSTACLE_GENERATE_ERROR            0x03
#define FAPA_OBSTACLE_OUTPUT_ERROR              0x01

#define FAPA_CAMERA_NO_DATA                     0x01
#define FAPA_OPENGL_ERROR                       0x01
#define FAPA_BEV_FPS_TOO_LOW                    0x02

#define FAPA_LOCATION_TIME_OUT                  0x01
#define FAPA_LOCATION_DATA_ERROR                0x02
#define FAPA_VEHICLE_SIGNAL_TIME_OUT            0x03
#define FAPA_VEHICLE_SIGNAL_ERROR               0x04
#define FAPA_PLAN_DATA_TIME_OUT                 0x05
#define FAPA_PLAN_DATA_ERROR                    0x06
#define FAPA_APSTATE_DATA_TIME_OUT              0x07
#define FAPA_APSTATE_DATA_ERROR                 0x08

#define FAPA_CONTROL_CPU_OCCUPATION_TOO_HIGH    0x01
#define FAPA_CONTROL_RUN_TIMEOUT                0x02

#define FAPA_L_BIAS_EXCEED_THRESHOLD            0x01
#define FAPA_YAW_BIAS_EXCEED_THRESHOLD          0x02
#define FAPA_LATERAL_CONTROL_OVERTIME           0x03

#define FAPA_CONTROL_DDS_SEND_FAIL              0x01

#define FAPA_HAS_NOR_GOT_SPOTS                  0x01
#define FAPA_INVALID_SPOTS                      0x02
#define FAPA_HAS_NOT_GOT_VEH_SIGNAL             0x03
#define FAPA_INVALID_VEH_SIGNAL                 0x04
#define FAPA_HAS_NOT_GOT_LOCATION               0x05
#define FAPA_INVALID_LOCATION                   0x06
#define FAPA_INVALID_PARKING_MODE               0x07
#define FAPA_INVALID_PARKING_SPACE_TYPE         0x08
#define FAPA_INVALID_SPOT_CORNERS               0x01
#define FAPA_INVALID_PARKING_CORD               0x02
#define FAPA_INVALID_PARKING_STATUS             0x03
#define FAPA_NOT_IN_PARKING_SECENARIOS          0x04
#define FAPA_INVALID_TURN_RADIUS                0x03
#define FAPA_HAS_NO_PATH                        0x04
#define FAPA_INVALID_PATH_SIZE                  0x05

#define	FCAMERA_FRAME            0x01
#define	FVEHICLE_SIGNAL          0x02
#define	FAVM_SETTINGS            0x03
#define	FUSS_DIST_RADAR          0x04
#define	FAVM_CALIB_STATUS        0x05
    
#define	FMEMORY_LEAK             0x01  
#define	FCPU_TOO_LOW             0x02
#define	FCPU_TOO_HIGH            0x03
#define	FFPS_TOO_LOW             0x04

#define	FNOT_FOUND_FILE_CAMERAS_CAMERA_3D_JSON                  0x01
#define	FNOT_FOUND_FILE_CAMERAS_CAMERA_AERIAL_JSON              0x02
#define	FNOT_FOUND_FILE_CAMERAS_CAMERA_SINGLE_FRONT_JSON        0x03
#define	FNOT_FOUND_FILE_CAMERAS_CAMERA_SINGLE_FRONT_WIDE_JSON   0x04
#define	FNOT_FOUND_FILE_CAMERAS_CAMERA_SINGLE_REAR_JSON         0x05
#define	FNOT_FOUND_FILE_CAMERAS_CAMERA_SINGLE_REAR_WIDE_JSON    0x06
#define	FNOT_FOUND_FILE_DDS_CONFIG                              0x07
#define	FNOT_FOUND_FILE_FONTS_SONG_TTF                          0x08
#define	FNOT_FOUND_FILE_OBJECTS_CAR_BLENDER_CAR_GLB             0x09
#define	FNOT_FOUND_FILE_OBJECTS_CAR_LIGHT_PNG                   0x0A
#define	FNOT_FOUND_FILE_SHADERS_ARC_PLANE_FS                    0x0B
#define	FNOT_FOUND_FILE_SHADERS_ARC_PLANE_VS                    0x0C
#define	FNOT_FOUND_FILE_SHADERS_ARC_WHEEL_FS                    0x0D
#define	FNOT_FOUND_FILE_SHADERS_ARC_WHEEL_VS                    0x0E
#define	FNOT_FOUND_FILE_SHADERS_ARC_WIDTH_FS                    0x0F
#define	FNOT_FOUND_FILE_SHADERS_ARC_WIDTH_VS                    0x10
#define	FNOT_FOUND_FILE_SHADERS_CALIBRATE_SELECT_FS             0x11
#define	FNOT_FOUND_FILE_SHADERS_CALIBRATE_SELECT_VS             0x12
#define	FNOT_FOUND_FILE_SHADERS_CALIBRATING_FS                  0x13
#define	FNOT_FOUND_FILE_SHADERS_CALIBRATING_VS                  0x14
#define	FNOT_FOUND_FILE_SHADERS_CAR_FS                          0x15
#define	FNOT_FOUND_FILE_SHADERS_CAR_VS                          0x16
#define	FNOT_FOUND_FILE_SHADERS_FONT_FS                         0x17
#define	FNOT_FOUND_FILE_SHADERS_FONT_VS                         0x18
#define	FNOT_FOUND_FILE_SHADERS_MAPPING_FS                      0x19
#define	FNOT_FOUND_FILE_SHADERS_MAPPING_VS                      0x1A
#define	FNOT_FOUND_FILE_SHADERS_RADAR_FS                        0x1B
#define	FNOT_FOUND_FILE_SHADERS_RADAR_VS                        0x1C
#define	FNOT_FOUND_FILE_SHADERS_RADAR_3D_FS                     0x1D
#define	FNOT_FOUND_FILE_SHADERS_RADAR_3D_VS                     0x1E
#define	FNOT_FOUND_FILE_TEXTURES_CALIBRATE_IMAGE_PNG            0x1F
#define	FNOT_FOUND_FILE_TEXTURES_NO_WARN_PNG                    0x20
#define	FNOT_FOUND_FILE_TEXTURES_RADAR_01_PNG                   0x21
#define	FNOT_FOUND_FILE_TEXTURES_RADAR_02_PNG                   0x22
#define	FNOT_FOUND_FILE_TEXTURES_RADAR_03_PNG                   0x23
#define	FNOT_FOUND_FILE_TEXTURES_WARN_1_PNG                     0x24

#define	FGLES_CAR_ERROR                 0x01
#define	FGLES_3D_BOWL_ERROR             0x02
#define	FGLES_AERIAL_ERROR              0x03
#define	FGLES_RADAR_SHIELD_ERROR        0x04
#define	FGLES_CAR_ARC_ERROR             0x05
#define	FGLES_TEXT_ERROR                0x06

#define	FAVM_STATUS                     0x01
#define	FAVM_CALIB_CTRL                 0x02
#define	FCAMERA_UYVY                    0x03

/***********ErrorCode List ************/

/*Fusion*/
#define	ENO1VMEA	        256	/* 获取不到前v量测结果 */             
#define	ENO7VMEA	        257	/* 获取不到5v量测结果*/
#define	ENOFRONTLANE	    258	/* 获取不到前v车道线*/
#define	ENOREARLANE	        259	/* 获取不到后v车道线*/
#define	EVEHICLEPBPARSE	    260	/* vehicle pb反序列化失败*/
#define	EFRONTLANEPBPARSE	261	/* front lane pb反序列化失败*/
#define	EREARLANEPBPARSE	262	/* rear lane pb反序列化失败*/
#define	ENOFRONTRADAR   	263	/* 获取不到前radar量测结果*/
#define	EFRONTRADARPBPARSE	264	/* 前radar pb反序列化失败*/
#define	ENOFRRADAR      	265	/* 获取不到fr radar量测结果*/
#define	ENOFLRADAR      	266	/* 获取不到fl radar量测结果*/
#define	ENORRRADAR         	267	/* 获取不到rr radar量测结果*/
#define	ENORLRADAR      	268	/* 获取不到rl radar量测结果*/
#define	ECRPBPARSE	        269	/* 角雷达pb反序列失败*/
#define	ENOEGOCAR	        270	/* 获取不到车身数据*/
#define	EEGOPBPARSE	        271	/* 车身数据pb反序列化失败*/
#define	EDTTOOLARGE     	272	/* 滤波dt时间差过大*/
#define	ENORVMEA	        273	/* 找不到后V测量结果*/

/*Camera_Lane*/
#define	ENOSHORTFRONT	    300	/* 获取不到前视短焦摄像头数据*/
#define	ENOLONGFRONT	    301	/* 获取不到前视长焦摄像头数据*/
#define	ENOREAR	            302	/* 获取不到后视摄像头数据*/
#define	ENOCHASIS	        303	/* 获取不到车身数据*/
#define	ESPEEDINVALID	    304	/* 速度数据异常*/
#define	EIMUINVALID	        305	/* IMU数据异常*/
#define	ECALIBINVALID	    306	/* 标定数据异常*/
#define	EACCINITFAILED	    307	/* 加速器模块初始化异常*/
#define	EMODELLOADFAILED	308 /*   模型加载失败*/
#define	EPITCHEST	        309 /*    pitch估计值异常*/
#define	EDELAY	            310 /*    计算延迟长时间过高*/
#define	ETMHIGHER	        311 /*    帧率过低 */
#define	ESPEEDHZ	        312 /*    速度数据帧率异常 */
#define	EIMUCALIB	        313/*     IMU无法标定 */
#define	ECAMCALIB	        314 /*    标定文件失效 */

/*Camera_Object*/
#define	ECAMERANOTSYNC      	351	 /* 相同时刻各V时间戳间隔太大 */
#define	ETIMINGNOTSTABLE	    352	 /* 单v前后时间戳相隔太大 */
		
#define	ENOCHASISDATA	        354	 /* 没有收到chasis_signals数据 */
#define	ENOCANSPEED	            355	 /* chasis_signals信号中没有vehicle_speed数据 */
#define	ENOIMUDATA      	    356	 /* 较长时间没有收到imu数据 */
#define	EIMUDATALOST    	    357	 /* imu数据有丢失 */
#define	ENOROADPERCEPTIONDATA	358	 /* 没有收到road_perception数据 */
#define	ENOUPDATEFORROAD	    359	 /* road_perception数据长时间没有更新 */
			
#define	ECAMERANOTCALIBRATED	361	 /* 摄像头未标定 */
#define	EMEMLEAK_OBJECT         362	 /* 有内存泄露 */
#define	ECPULOW_OBJECT	        363	 /* cpu占用率过低 */
#define	ECPUHIGH_OBJECT        	364	 /* cpu占用率过高 */
#define	EFPSLOW	                365	 /* 帧率过低 */
#define	EFORWARDTIMEOUT	        366	 /* 模型调用时间超过阈值 */
#define	EFORWARDFAILURE	        367	 /* 模型调用失败 */
#define	EPOSTPTIMEOUT_OBJECT	368	 /* 后处理时间超过阈值 */
#define	ESENDFAILED	            369	 /* 发送失败 */
#define	ECAMERADATALOST1	    370  /* 370 ~ 376	第i ([0, 6))路摄像头数据有丢失 */
#define	ECAMERADATALOST2	    371
#define	ECAMERADATALOST3	    372
#define	ECAMERADATALOST4	    373
#define	ECAMERADATALOST5	    374
#define	ECAMERADATALOST6	    375
#define	ECAMERADATALOST7	    376


#define	ECAMERACALIBRATEFAILED1	     390       /*390 ~396	第i([0, 7))路摄像头标定参数误差大*/
#define	ECAMERACALIBRATEFAILED2      391
#define	ECAMERACALIBRATEFAILED3      392
#define	ECAMERACALIBRATEFAILED4      393
#define	ECAMERACALIBRATEFAILED5      394
#define	ECAMERACALIBRATEFAILED6      395
#define	ECAMERACALIBRATEFAILED7      396


/*Planing*/
#define	ENOFRONTLANE_PLAN	        451 /* 获取不到前v车道线数据*/
#define	ENOFUSION	                452 /* 获取不到融合数据*/
#define	ENOVEHICLESINGAL    	    453 /* 获取不到车身数据*/
#define	EFRONTLANEPBPARSE_PLAN	    454 /* 车道线数据数据 pb反序列化失败*/
#define	EFUSIONPBPARSE      	    455 /* 融合数据 pb反序列化失败*/
#define	EVEHICLESINGALPBPARSE	    456 /* 车身数据 pb反序列化失败*/
#define	EMEMLEAK_PLANING	        457 /* 有内存泄露*/
#define	ECPULOW_PLANING	            458 /* cpu占用率过低*/
#define	ECPUHIGH_PLANING            459 /* cpu占用率过高*/
#define	EPOSTPTIMEOUT_PLANING       460 /* 运行时间超过阈值*/
#define	EDNPCOMMON          	    461 /* 模块调用时间超过阈值*/
#define	EDNPEGOME	                462 /* 模块调用时间超过阈值*/
#define	EDNPOBJECTTP	            463 /* 模块调用时间超过阈值*/
#define	EDNPVISONP	                464 /* 模块调用时间超过阈值*/
#define	EDNPSTEERINGPP	            465 /* 模块调用时间超过阈值*/
#define	EDNPDECISION	            466 /* 模块调用时间超过阈值*/
#define	EDNPLATERALPLANNER	        467 /* 模块调用时间超过阈值*/
#define	EDNPLONGITUDINAL	        468 /* 模块调用时间超过阈值*/
#define	EDNPSIDEFEATURE	            469 /* 模块调用时间超过阈值*/
#define	EDNPEHR	                    470 /* 模块调用时间超过阈值*/
#define	ELIBFLOWSEND                471 /* libflow发送失败*/
#define	ELIBTIMEOUT	                472 /* libflow模块调用时间超过阈值*/
#define	EDDSSEND	                473 /* dds发送失败,模块调用时间超过阈值*/
#define	EDDSTIMEOUT                 474 /* dds模块调用时间超过阈值*/                                        

/*SOC-hardware*/

#define ESOCNODATA                   0x01     /*无数据*/
#define EDROPFRAME                   0x02     /*丢帧*/
#define EINITFAILED                  0x01     /*初始化失败*/
#define ESHORTCIRCUIT                0x01     /*短路*/
#define EOPENCIRCUIT                 0x02     /*开路*/
#define EUNREACHABLE                 0x01     /*无法访问*/
#define ECONFIGERROR                 0x02     /*配置错误*/
#define ECPUHIGHTEMP                 0x01     /*过温保护*/
#define ELINKDOWN                    0x01     /*以太网link down*/
#define ERADARFAULD                  0x01     /*DTC雷达故障*/
#define ERADARTIMEOUT                0x02     /*DTC雷达通讯超时*/
#define ERADARCALIB                  0x03     /*DTC雷达未标定或标定失败*/
#define ERADARNODATA                 0x04     /*DTC雷达数据失效*/
#define ERADARBLOCK                  0x05     /*DTC雷达遮挡*/
#define ECANBUSOFF                   0x06     /*DTC can busoff*/
#define ELOSTNODESPEED               0x01     /*车速节点丢失*/
#define ELOSTNODETRUNLIGHTNODE       0x02     /*转向灯节点丢失*/
#define ELOSNODETYAWRATE             0x03     /*车身yaw rate节点丢失*/
#define ELANENORESPOND               0x01     /*车道行泊状态切换不响应*/
#define EFUSIONNORESPOND             0x03     /*融合行泊状态切换不响应*/
#define EPLANNORESPOND               0x04     /*规划行泊状态切换不响应*/
#define ESAGNORESPOND                0x06     /*SAG行泊状态切换不响应*/
#define EAPANORESPOND                0x0b     /*apa行泊状态切换不响应*/
#define EOBJECTNORESPOND             0x15     /*车辆行泊状态切换不响应*/
#define EMCUNORESPOND                0x16     /*mcu行泊状态切换不响应*/


/*PHM*/
#define ENOALIVETIMEOUT              700      /*AliveTask 超时*/
#define ENOALIVEOVERRUN              701      /*AliveTask 超过允许上报次数*/
#define ENOWATCHDOGTIMEOUT           702      /*心跳上报超时，未在规定时间内收到心跳信息*/
#define ENOAPPEXIT                              703     /* process abort */


/*apa*/
#define ELOCIVEHSTIMEOUT            500       /*Apa_Location_input_获取车身数据超时*/
#define ELOCISUBVEHSIGERROR         501       /*Apa_Location_input_无法订阅车身数据*/
#define ELOCIVEHSIGDATAERROR        502       /*Apa_Location_input_车身数据异常*/
#define ELOCIIMUDATATIMEOUT         503       /*Apa_Location_input_获取imu数据超时*/
#define ELOCISUBIMUDATAERROR        504       /*Apa_Location_input_无法订阅imu数据*/
#define ELOCIIMUDATAERROR           505       /*Apa_Location_input_imu数据异常*/

#define ELOCSYSRUNTIMEOUT           509       /*Apa_Location_system_运行时间超过阈值*/
#define ELOCPROIMUPROC              510       /*Apa_Location_procedure_imu积分模块调用时间超过阈值*/
#define ELOCPROODOMPROC             511       /*Apa_Location_procedure_轮速计积分时间超过阈值*/
#define ELOCIMUINITERROR            512       /*Apa_Location_procedure_imu初始化失败*/
#define ELOCODDSERROR               513       /*Apa_Location_output_DDS_发送失败*/
#define ELOCOLOSTLOCATION           514       /*Apa_Location_output__位置丢失*/

#define EAVMICAMNOTCALIB            515       /*Apa_Avm_input_AVM未标定*/
#define EAVMILOADCONFIGERROR        516       /*Apa_Avm_input_标定参数文件加载失败*/

#define EAVMPROCAM1CALIBFAILED      525       /*Apa_Avm_procedure_第1路摄像头标定参数误差大*/
#define EAVMPROCAM2CALIBFAILED      526       /*Apa_Avm_procedure_第2路摄像头标定参数误差大*/
#define EAVMPROCAM3CALIBFAILED      527       /*Apa_Avm_procedure_第3摄像头标定参数误差大*/
#define EAVMPROCAM4CALIBFAILED      528       /*Apa_Avm_procedure_第4路摄像头标定参数误差大*/
#define EAVMPRO1CHNOTCONNER         529       /*Apa_Avm_procedure_第1路棋盘格角点检测失败*/
#define EAVMPRO2CHNOTCONNER         530       /*Apa_Avm_procedure_第2路棋盘格角点检测失败*/
#define EAVMPRO3CHNOTCONNER         531       /*Apa_Avm_procedure_第3路棋盘格角点检测失败*/
#define EAVMPRO4CHNOTCONNER         532       /*Apa_Avm_procedure_第4路棋盘格角点检测失败*/
#define EAVMO1CHSAVECALIBFAILED     533       /*Apa_Avm_output_第1路摄像头算法结果保存失败*/
#define EAVMO2CHSAVECALIBFAILED     534       /*Apa_Avm_output_第2路摄像头算法结果保存失败*/
#define EAVMO3CHSAVECALIBFAILED     535       /*Apa_Avm_output_第3路摄像头算法结果保存失败*/
#define EAVMO4CHSAVECALIBFAILED     536       /*Apa_Avm_output_第4路摄像头算法结果保存失败*/

#define EPPISIGHTINPUTABNORMAL      537       /*Apa_Parkingspace_Postprocess_input_视觉感知输入异常*/
#define EPPISIGHTSUBERROR           538       /*Apa_Parkingspace_Postprocess_input_无法订阅感知输入*/
#define EPPISIGHTINPUTTIMEOUT       539       /*Apa_Parkingspace_Postprocess_input_视觉感知输入超时*/
#define EPPIRADARINPUTABNORMAL      540       /*Apa_Parkingspace_Postprocess_input_超声波雷达感知输入异常*/
#define EPPIRADARSUBERROR           541       /*Apa_Parkingspace_Postprocess_input_无法订阅超声波雷达输入*/
#define EPPIRADARINPUTTIMEOUT       542       /*Apa_Parkingspace_Postprocess_input_超声波雷达感知输入超时*/
#define EPPIVEHICLEINPUTABNORMAL    543       /*Apa_Parkingspace_Postprocess_input_车身信号输入异常*/
#define EPPIVEHICLESUBERROR         544       /*Apa_Parkingspace_Postprocess_input_无法订阅车身信号输入*/
#define EPPIVEHICLEINPUTTIMEOUT     545       /*Apa_Parkingspace_Postprocess_input_车身信号输入超时*/

#define EPPSYSRUNTIMEOUT            549       /*Apa_Parkingspace_Postprocess_system_运行超时*/
#define EPPODDSERROR                555       /*Apa_Parkingspace_Postprocess_output_发送失败*/
#define EINFERINOBEVINPUT           557       /*Apa_Inference_input_没有鸟瞰图输入*/
#define EINFERIBEVINPUTABNORMAL     558       /*Apa_Inference_input_鸟瞰图输入异常*/
#define EINFERSYSRUNTIMEOUT         562       /*Apa_Inference_system_运行超时*/
#define EINFERPROACCERROR           563         /*Apa_Inference_procedure_tad4加速器错误*/
#define EINFERPROCNNERROR           564       /*Apa_Inference_procedure_cnn处理错误*/
#define EINFERPROFPSLOW             566       /*Apa_Inference_procedure_帧率过低*/
#define EINFERODDSERROR             567       /*Apa_Inference_output_转化成dds消息失败*/

#define EBEVICAMNOTDATA             577       /*Apa_Bev_input_获取不到四路鱼眼数据*/
#define EBEVCONFIGFILENOTEXIST      578         /*Apa_Bev_confige_file不存在*/
#define EBEVOPENVXERROR             579         /*Apa_Bev_openvx内部错误*/
#define EBEVPROFPSLOW               581       /*Apa_Bev_procedure_帧率过低*/
#define EBEVODDSERROR               582       /*Apa_Bev_output_转化成dds消息失败*/

#define ECONILOCATIONTIMEOUT        583       /*Apa_Control_input_获取定位数据超时*/
#define ECONISUBLOCATIONDATAERROR   584       /*Apa_Control_input_无法订阅定位数据*/
#define ECONILOCATIONDATAERROR      585       /*Apa_Control_input_定位数据异常*/
#define ECONIVEHSIGTIMEOUT          586       /*Apa_Control_input_获取车身数据超时*/
#define ECONIVEHSUBDATAERROR        587       /*Apa_Control_input_无法订阅车身数据*/
#define ECONIVEHSIGDATAERROR        588       /*车身数据信号异常*/
#define ECONIPLANDATATIMEOUT        589       /*Apa_Control_input_获取plan数据超时*/
#define ECONIPLASUBNDATAERROR       590       /*Apa_Control_input_无法订阅plan数据*/
#define ECONIPLANDATAERROR          591       /*Apa_Control_input_plan数据异常*/

#define ECONSYSRUNTIMEOUT           595       /*运行时间超过阈值*/
#define ECONIProLBiasExceed         596       /*Apa_Control_procedure_横向偏差超出阈值*/
#define ECONIProYawBiasExceed       597       /*Apa_Control_procedure_航向偏差超出阈值*/
#define ECONPROLATERALCONOVERTIME   598       /*Apa_Control_procedure_横向控制解算时间超出阈值*/
#define ECONOLIBFLOWERROR           599       /*Apa_Control_output_发送失败*/
#define ECONODDSERROR               600       /*Apa_Control_output_发送失败*/

#define EPALNINOTSPOTS              601       /*Apa_Planning_input_无法获取车位信息*/
#define EPALNIINVALIDSPOTS          602       /*Apa_Planning_input_无效的车位信息*/
#define EPALNINOTVEHSIGNAL          603       /*Apa_Planning_input_无法获取车身数据*/
#define EPALNINOTLOCATION           605       /*Apa_Planning_input_无法获取定位信息*/
#define EPALNIINVALIDLOCATION       606       /*Apa_Planning_input_invalid_location*/
#define EPALNIINVALIDPARKMODE       607       /*Apa_Planning_input_泊车类型异常*/
#define EPALNSYSRUNTIMEOUT          612       /*Apa_Planning_system_运行时间超过阈值*/
#define EPALNPROINVALIDSPOTCORNERS  613       /*Apa_Planning_procedure_车位角点数据异常*/
#define EPALNPROINVALIDPARKINGSTATUS   615    /*Apa_Planning_procedure_无效的泊车状态*/
#define EPALNODDSERROR                 618    /*Apa_Planning_output_发送失败*/
#define EPALNOINVALIDTURNRADIUS        619    /*Apa_Planning_output_转弯半径不合理*/
#define EPALNONOPATH                   620    /*Apa_Planning_output_没有路径输出*/
#define EPALNOINVALIDPATHSIZE          621    /*Apa_Planning_output_路径段数不合理*/

/*avm-packing*/
#define	ERR_NO_CAMERA_FRAME	                    801	     /*	收不到图片	*/
#define	ERR_NO_VEHICLE_SIGNAL	                802      /*	收不到车身信号	*/
#define	ERR_NO_AVM_SETTINGS	                    803	     /*	收不到控制信号	*/
#define	ERR_NO_USS_DIST_RADAR	                804	     /*	收不到雷达信号	*/
#define	ERR_NO_AVM_CALIB_STATUS	                805	     /*	收不到标定状态信号	*/
#define	ERR_MEMORY_LEAK	                        806	     /*	内存泄漏	*/
#define	ERR_CPU_TOO_LOW	                        807	     /*	CPU太低	*/
#define	ERR_CPU_TOO_HIGH	                    808	     /*	CPU过高	*/
#define	ERR_FPS_TOO_LOW	                        809	     /*	fps过低	*/
#define	NOT_FOUND_FILE_CAMERAS_CAMERA_3D_JSON	810	     /*	缺少文件resources/cameras/camera_3d.json	*/
#define	NOT_FOUND_FILE_CAMERAS_CAMERA_AERIAL_JSON	            811 	/*	缺少文件resources/cameras/camera_aerial.json	*/
#define	NOT_FOUND_FILE_CAMERAS_CAMERA_SINGLE_FRONT_JSON	        812 	/*	缺少文件resources/cameras/camera_single_front.json	*/
#define	NOT_FOUND_FILE_CAMERAS_CAMERA_SINGLE_FRONT_WIDE_JSON	813 	/*	缺少文件resources/cameras/camera_single_front_wide.json	*/
#define	NOT_FOUND_FILE_CAMERAS_CAMERA_SINGLE_REAR_JSON	        814 	/*	缺少文件resources/cameras/camera_single_rear.json	*/
#define	NOT_FOUND_FILE_CAMERAS_CAMERA_SINGLE_REAR_WIDE_JSON	    815 	/*	缺少文件resources/cameras/camera_single_rear_wide.json	*/
#define	NOT_FOUND_FILE_DDS_CONFIG	                            816 	/*	缺少文件resources/dds_config/dds_config.json	*/
#define	NOT_FOUND_FILE_FONTS_SONG_TTF	                        817 	/*	缺少文件resources/fonts/song.ttf	*/
#define	NOT_FOUND_FILE_OBJECTS_CAR_BLENDER_CAR_GLB	            818 	/*	缺少文件resources/objects/car/blender_car.glb	*/
#define	NOT_FOUND_FILE_OBJECTS_CAR_LIGHT_PNG    	819 	/*	缺少文件resources/objects/car/car_light.png	*/
#define	NOT_FOUND_FILE_SHADERS_ARC_PLANE_FS     	820 	/*	缺少文件resources/shaders/arc_plane.fs	*/
#define	NOT_FOUND_FILE_SHADERS_ARC_PLANE_VS     	821 	/*	缺少文件resources/shaders/arc_plane.vs	*/
#define	NOT_FOUND_FILE_SHADERS_ARC_WHEEL_FS     	822 	/*	缺少文件resources/shaders/arc_wheel.fs	*/
#define	NOT_FOUND_FILE_SHADERS_ARC_WHEEL_VS     	823 	/*	缺少文件resources/shaders/arc_wheel.vs	*/
#define	NOT_FOUND_FILE_SHADERS_ARC_WIDTH_FS     	824 	/*	缺少文件resources/shaders/arc_width.fs	*/
#define	NOT_FOUND_FILE_SHADERS_ARC_WIDTH_VS     	825 	/*	缺少文件resources/shaders/arc_width.vs	*/
#define	NOT_FOUND_FILE_SHADERS_CALIBRATE_SELECT_FS	826 	/*	缺少文件resources/shaders/calibrate_select.fs	*/
#define	NOT_FOUND_FILE_SHADERS_CALIBRATE_SELECT_VS	827 	/*	缺少文件resources/shaders/calibrate_select.vs	*/
#define	NOT_FOUND_FILE_SHADERS_CALIBRATING_FS 	    828 	/*	缺少文件resources/shaders/calibrating.fs	*/
#define	NOT_FOUND_FILE_SHADERS_CALIBRATING_VS	    829 	/*	缺少文件resources/shaders/calibrating.vs	*/
#define	NOT_FOUND_FILE_SHADERS_CAR_FS	            830 	/*	缺少文件resources/shaders/car.fs	*/
#define	NOT_FOUND_FILE_SHADERS_CAR_VS	            831 	/*	缺少文件resources/shaders/car.vs	*/
#define	NOT_FOUND_FILE_SHADERS_FONT_FS	            832 	/*	缺少文件resources/shaders/font.fs	*/
#define	NOT_FOUND_FILE_SHADERS_FONT_VS	            833 	/*	缺少文件resources/shaders/font.vs	*/
#define	NOT_FOUND_FILE_SHADERS_MAPPING_FS	        834 	/*	缺少文件resources/shaders/mapping.fs	*/
#define	NOT_FOUND_FILE_SHADERS_MAPPING_VS	        835 	/*	缺少文件resources/shaders/mapping.vs	*/
#define	NOT_FOUND_FILE_SHADERS_RADAR_FS	            836 	/*	缺少文件resources/shaders/radar.fs	*/
#define	NOT_FOUND_FILE_SHADERS_RADAR_VS	            837 	/*	缺少文件resources/shaders/radar.vs	*/
#define	NOT_FOUND_FILE_SHADERS_RADAR_3D_FS	        838 	/*	缺少文件resources/shaders/radar_3d.fs	*/
#define	NOT_FOUND_FILE_SHADERS_RADAR_3D_VS	        839 	/*	缺少文件resources/shaders/radar_3d.vs	*/
#define	NOT_FOUND_FILE_TEXTURES_CALIBRATE_IMAGE_PNG	840 	/*	缺少文件resources/textures/calibrate_image.png	*/
#define	NOT_FOUND_FILE_TEXTURES_NO_WARN_PNG	        841 	/*	缺少文件resources/textures/no_warn.png	*/
#define	NOT_FOUND_FILE_TEXTURES_RADAR_01_PNG    	842 	/*	缺少文件resources/textures/radar_01.png	*/
#define	NOT_FOUND_FILE_TEXTURES_RADAR_02_PNG    	843 	/*	缺少文件resources/textures/radar_02.png	*/
#define	NOT_FOUND_FILE_TEXTURES_RADAR_03_PNG    	844 	/*	缺少文件resources/textures/radar_03.png	*/
#define	NOT_FOUND_FILE_TEXTURES_WARN_1_PNG	        845 	/*	缺少文件resources/textures/warn_1.png	*/
#define	GLES_CAR_ERROR	        846     /*	车辆模型出错	*/
#define	GLES_3D_BOWL_ERROR	    847   	/*	3D碗模型出错	*/
#define	GLES_AERIAL_ERROR	    848   	/*	鸟瞰图模型出错	*/
#define	GLES_RADAR_SHIELD_ERROR	849   	/*	雷达盾牌模型出错	*/
#define	GLES_CAR_ARC_ERROR	    850   	/*	车轮轨迹线模型出错	*/
#define	GLES_TEXT_ERROR	        851   	/*	文字渲染出错	*/
#define	ERR_OUT_AVM_STATUS	    852   	/*	输出avm_status出错	*/
#define	ERR_OUT_AVM_CALIB_CTRL	853   	/*	发送控制信号给标定程序出错	*/
#define	ERR_OUT_CAMERA_UYVY	    854   	/*	发送图片数据出错	*/

/*parking*/
#define ESURROUNDCAMERAERROR        501     /*环视摄像头数据读取异常*/
#define EUSSERROR                   502     /*超声波数据读取异常*/
#define ECHASISERROR                503     /*车身底盘数据读取异常*/
#define EUERSDATAERROR              504     /*PrivateCAN用户交互数据读取异常*/
#define ECAMERACALIBCFGERROR        505     /*环视摄像头标定参数读取异常*/
#define EUSSCALIBCFGERROR           506     /*超声波标定参数读取异常*/
#define ELOWSPEEDAEBCLOSETIMEOUT    507     /*低速AEB关闭超时*/
#define EAPAPROCESSERROR            508     /*泊车执行器异常*/
#define ECHASISPROCESSERROR         509     /*底盘执行器EPS、ESP异常*/
#define EAPAPROCESSECPUHIGH         510     /*泊车执行器CPU LOADING高*/
#define EAPAPROCESSEMEMHIGH         511     /*泊车执行器内存LOADING 高*/
#define ESYSTEMCPUHIGH              512     /*域控系统CPU LOADING高*/
#define ESYSTEMMEMHIGH              513     /*域控系统内存LOADING 高*/

};//namespace minieye 
