#pragma once

namespace minieye {

namespace EM {

typedef enum EventType {

    EVENT_TYPE_MIN = 0,                             
    EVENT_TYPE_ENOCHASISDATA,                   /* 没有收到chasis_signals数据 */
    EVENT_TYPE_ENOCANSPEED,                     /*chasis_signals信号中没有vehicle_speed数据 */
    EVENT_TYPE_ECAMERANOTCALIBRATED,            /*摄像头未标定*/
    EVENT_TYPE_EFORWARDFAILURE,                 /*模型调用失败*/
    EVENT_TYPE_ESENDFAILED,                     /*算法结果发送失败*/
    EVENT_TYPE_ECAMERADATALOST1,                /*Front Narrow摄像头数据有丢失*/
    EVENT_TYPE_ECAMERADATALOST2,                /*Rear 摄像头数据有丢失*/
    EVENT_TYPE_ECAMERADATALOST3,                /*Right Rear摄像头数据有丢失*/
    EVENT_TYPE_ECAMERADATALOST4,                /*Front Wide摄像头数据有丢失*/
    EVENT_TYPE_ECAMERADATALOST5,                /*Left Rear摄像头数据有丢失*/
    EVENT_TYPE_ECAMERADATALOST6,                /*Left Front摄像头数据有丢失*/
    EVENT_TYPE_ECAMERADATALOST7,                /*Right Front摄像头数据有丢失*/
    EVENT_TYPE_ECAMERACALIBRATEFAILED1,         /*Front Narrow摄像头标定参数误差大*/
    EVENT_TYPE_ECAMERACALIBRATEFAILED2,         /*Rear camare摄像头标定参数误差大*/
    EVENT_TYPE_ECAMERACALIBRATEFAILED3,         /*Right Rear摄像头标定参数误差大*/
    EVENT_TYPE_ECAMERACALIBRATEFAILED4,         /*Front Wide摄像头标定参数误差大*/
    EVENT_TYPE_ECAMERACALIBRATEFAILED5,         /*Left Rear摄像头标定参数误差大*/
    EVENT_TYPE_ECAMERACALIBRATEFAILED6,         /*Left Front摄像头标定参数误差大*/
    EVENT_TYPE_ECAMERACALIBRATEFAILED7,         /*Right Front摄像头标定参数误差大*/

    EVENT_TYPE_OBJECT_MAX = 30,

    EVENT_TYPE_ENOLONGFRONT,                    /*获取不到前视摄像头数据*/
    EVENT_TYPE_ENOREAR,                         /*获取不到后视摄像头数据*/
    EVENT_TYPE_ENOCHASIS,                       /*获取不到车身数据*/
    EVENT_TYPE_ESPEEDINVALID,                   /*速度数据异常*/
    EVENT_TYPE_ECALIBINVALID,                   /*摄像头标定数据异常*/
    EVENT_TYPE_EACCINITFAILED,                  /*加速器模块初始化异常*/
    EVENT_TYPE_EMODELLOADFAILED,                /*模型加载失败*/

    EVENT_TYPE_LANE_MAX = 40,

    EVENT_TYPE_ENO1VMEA,                        /*获取不到前v量测结果*/
    EVENT_TYPE_ENOFRONTLANE,                    /*获取不到前v车道线*/
    EVENT_TYPE_EVEHICLEPBPARSE,                 /*vehicle pb反序列化失败*/
    EVENT_TYPE_EFRONTLANEPBPARSE,               /*front lane pb反序列化失败*/
    EVENT_TYPE_EREARLANEPBPARSE,                /*rear lane pb反序列化失败*/
    EVENT_TYPE_ENOFRONTRADAR,                   /*获取不到前radar量测结果*/
    EVENT_TYPE_EFRONTRADARPBPARSE,              /*前radar pb反序列化失败*/
    EVENT_TYPE_ENOFRRADAR,                      /*获取不到fr radar量测结果*/
    EVENT_TYPE_ENOFLRADAR,                      /*获取不到fl radar量测结果*/
    EVENT_TYPE_ENORRRADAR,                      /*获取不到rr radar量测结果*/
    EVENT_TYPE_ENORLRADAR,                      /*获取不到rl radar量测结果*/
    EVENT_TYPE_ECRPBPARSE,                      /*角雷达pb反序列失败*/
    EVENT_TYPE_ENOEGOCAR,                       /*获取不到车身数据*/
    EVENT_TYPE_EEGOPBPARSE,                     /*车身数据pb反序列化失败*/

    EVENT_TYPE_FUSION_MAX = 60,

    EVENT_TYPE_ENOFRONTLANE_PLAN,               /*获取不到前v车道线数据*/
    EVENT_TYPE_ENOFUSION,                       /*获取不到融合数据*/
    EVENT_TYPE_ENOVEHICLESINGAL,                /*获取不到车身数据*/
    EVENT_TYPE_EFRONTLANEPBPARSE_PLAN,          /*车道线数据数据 pb反序列化失败*/
    EVENT_TYPE_EFUSIONPBPARSE,                  /*融合数据 pb反序列化失败*/
    EVENT_TYPE_EVEHICLESINGALPBPARSE,           /*车身数据 pb反序列化失败*/
    EVENT_TYPE_ELIBFLOWSEND,                    /*LIBFLOW发送失败*/
    EVENT_TYPE_EDDSSEND,                        /*DDS发送失败*/

    EVENT_TYPE_PLANING_MAX = 70,

    EVENT_TYPE_FRONTCAMFAULT,                   /*前摄像头故障*/
    EVENT_TYPE_REARCAMFAULT,                    /*后摄像头故障*/
    EVENT_TYPE_AVMFRONTCAMFAULT,                /*avm 前摄像头故障*/
    EVENT_TYPE_AVMREARCAMFAULT,                 /*avm 后摄像头故障*/
    EVENT_TYPE_AVMLEFTCAMFAULT,                 /*avm 左摄像头故障*/
    EVENT_TYPE_AVMRIGHTCAMFAULT,                /*avm 右摄像头故障*/
    EVENT_TYPE_CPUOVERHIGH,                     /*cpu 温度过高*/
    EVENT_TYPE_FRONTRADARFAULT,                 /*前雷达故障*/
    EVENT_TYPE_FRONTRIGHTRADARFAULT,            /*Front right雷达故障*/
    EVENT_TYPE_FRONTLEFTRADARFAULT,             /*Front left雷达故障*/
    EVENT_TYPE_REARRIGHTRADARFAULT,             /*Rear right 雷达故障*/
    EVENT_TYPE_REARLEFTRADARFAULT,              /*rear left雷达故障*/
    EVENT_TYPE_IMU,
    EVENT_TYPE_INTELLINK,
    EVENT_TYPE_PRIVATECANBUSOFF,
    EVENT_TYPE_FRONTCAMSHELTER,
    EVENT_TYPE_REARCAMSHELTER,
    EVENT_TYPE_SOCHARDWARE_MAX = 90,

    EVENT_TYPE_LanePHMTimeOut,
    EVENT_TYPE_ObjectPHMTimeOut,
    EVENT_TYPE_FusionPHMTimeOut,
    EVENT_TYPE_APAPHMTimeOut,
    EVENT_TYPE_CanOutPHMTimeOut,
    EVENT_TYPE_ChassisPHMTimeOut,
    EVENT_TYPE_RadarPHMTimeOut,
    EVENT_TYPE_PrivateCanOutPHMTimeOut,
    EVENT_TYPE_USS_RadarPHMTimeOut,
    EVENT_TYPE_AngleRadarPHMTimeOut,
    EVENT_TYPE_IMUPHMTimeOut,
    EVENT_TYPE_AVM_ParkingPHMTimeOut,
    EVENT_TYPE_AVM_CAMPHMTimeOut,
    EVENT_TYPE_FrontCAMPHMTimeOut,
    EVENT_TYPE_CSITXPHMTimeOut,
    EVENT_TYPE_CalibratePHMTimeOut,
    EVENT_TYPE_AVM_CalibPHMTimeOut,
    EVENT_TYPE_LaneProcessEXIT,                         /*车道进程退出*/
    EVENT_TYPE_ObjectProcessEXIT,                       /*车辆进程退出*/

    EVENT_TYPE_PHMTIMEOUT_MAX = 120,                      /*心跳上报超时，未在规定时间内收到心跳信息*/

    EVENT_TYPE_PARKING_APASENSORERROR,                         /*传感异常*/
    EVENT_TYPE_PARKING_PILOTPROCESSERROR,                      /*超声波数据读取异常*/
    EVENT_TYPE_PARKING_APAPROCESSERROR,                        /*执行器异常*/
    EVENT_TYPE_PARKING_CHASISPROCESSERROR,                     /*底盘执行器EPS、ESP异常*/
    EVENT_TYPE_PARKING_SYSTEMERROR,                            /*系统异常*/
    
    EVENT_TYPE_PARKING_MAX = 130,

    EVENT_TYPE_IMUDATAERROR,                    /*IMU 数据异常*/
    EVENT_TYPE_TMHIGHER,                        /*帧率过低*/
    EVENT_TYPE_PITCHEST,                        /*pitch估计值异常*/
    EVENT_TYPE_IMUCALIB,                        /*IMU无法标定*/
    EVENT_TYPE_CAMCALIB,                        /*相机标定文件失效*/

    EVENT_TYPE_MCUR5_FAILED,                     /*R5运行异常*/
    EVENT_TYPE_DSPC66_FAILED,                    /*C66 运行异常*/
    EVENT_TYPE_DSPC71_FAILED,                    /*C71 运行异常*/
    EVENT_TYPE_LOSTSPEEDNODE,                    /*车速节点丢失*/
    EVENT_TYPE_LANENOREPOND,                     /*行泊模式切换车道不响应 */
    EVENT_TYPE_FUSIONNOREPOND,                   /*行泊模式切换融合不响应*/
    EVENT_TYPE_PLANNOREPOND,                     /*行泊模式切换规划不响应*/
    EVENT_TYPE_APANOREPOND,                      /*行泊模式切换apa不响应*/
    EVENT_TYPE_VEHINOREPOND,                     /*行泊模式切换车辆不响应*/
    EVENT_TYPE_MCUNOREPOND,                      /*行泊模式切换mcu不响应*/

    EVENT_TYPE_TSR_STATE,               /*功能降级*/
    EVENT_TYPE_IHBC_STATE,
    EVENT_TYPE_READY2GO_STATE,

    EVENT_TYPE_AroundCameraCalib,
    EVENT_TYPE_SAGNOREPOND,                     /*行泊模式切换sag不响应*/
    EVENT_TYPE_MAX,

    EVENT_TYPE_MISC_MAX = 152,

    EVENT_TYPE_APA_LOCIVEHSTIMEOUT,              /*Apa_Location_input_获取车身数据超时*/    
    EVENT_TYPE_APA_LOCISUBVEHSIGERROR,           /*Apa_Location_input_无法订阅车身数据*/
    EVENT_TYPE_APA_LOCIVEHSIGDATAERROR,          /*Apa_Location_input_车身数据异常*/
    EVENT_TYPE_APA_LOCIIMUDATATIMEOUT,           /*Apa_Location_input_获取imu数据超时*/
    EVENT_TYPE_APA_LOCISUBIMUDATAERROR,          /*Apa_Location_input_无法订阅imu数据*/
    EVENT_TYPE_APA_LOCIIMUDATAERROR,             /*Apa_Location_input_imu数据异常*/
    EVENT_TYPE_APA_LOCSYSRUNTIMEOUT,                   /*Apa_Location_system_运行时间超过阈值*/
    EVENT_TYPE_APA_LOCPROIMUPROC,                      /*Apa_Location_procedure_imu积分模块调用时间超过阈值*/
    EVENT_TYPE_APA_LOCPROODOMPROC,                     /*Apa_Location_procedure_轮速计积分时间超过阈值*/
    EVENT_TYPE_APA_LOCODDSERROR  = 162,                 /*Apa_Location_output_DDS_发送失败*/
    EVENT_TYPE_APA_LOCOLOSTLOCATION,                   /*Apa_Location_output__位置丢失*/
    EVENT_TYPE_APA_AVMICAMNOTCALIB,                    /*Apa_Avm_input_AVM未标定*/
    EVENT_TYPE_APA_AVMILOADCONFIGERROR,                /*Apa_Avm_input_标定参数文件加载失败*/
    EVENT_TYPE_APA_AVMPROCAM1CALIBFAILED,              /*Apa_Avm_procedure_第1路摄像头标定参数误差大*/
    EVENT_TYPE_APA_AVMPROCAM2CALIBFAILED,              /*Apa_Avm_procedure_第2路摄像头标定参数误差大*/
    EVENT_TYPE_APA_AVMPROCAM3CALIBFAILED,              /*Apa_Avm_procedure_第3摄像头标定参数误差大*/
    EVENT_TYPE_APA_AVMPROCAM4CALIBFAILED,              /*Apa_Avm_procedure_第4路摄像头标定参数误差大*/
    EVENT_TYPE_APA_AVMPRO1CHNOTCONNER,                 /*Apa_Avm_procedure_第1路棋盘格角点检测失败*/
    EVENT_TYPE_APA_AVMPRO2CHNOTCONNER,                 /*Apa_Avm_procedure_第2路棋盘格角点检测失败*/
    EVENT_TYPE_APA_AVMPRO3CHNOTCONNER = 172,           /*Apa_Avm_procedure_第3路棋盘格角点检测失败*/
    EVENT_TYPE_APA_AVMPRO4CHNOTCONNER,                 /*Apa_Avm_procedure_第4路棋盘格角点检测失败*/
    EVENT_TYPE_APA_AVMO1CHSAVECALIBFAILED,             /*Apa_Avm_output_第1路摄像头算法结果保存失败*/
    EVENT_TYPE_APA_AVMO2CHSAVECALIBFAILED,             /*Apa_Avm_output_第2路摄像头算法结果保存失败*/
    EVENT_TYPE_APA_AVMO3CHSAVECALIBFAILED,             /*Apa_Avm_output_第3路摄像头算法结果保存失败*/
    EVENT_TYPE_APA_AVMO4CHSAVECALIBFAILED,             /*Apa_Avm_output_第4路摄像头算法结果保存失败*/
    EVENT_TYPE_APA_PPISIGHTINPUTABNORMAL,              /*Apa_Parkingspace_Postprocess_input_视觉感知输入异常*/
    EVENT_TYPE_APA_PPISIGHTSUBERROR,                   /*Apa_Parkingspace_Postprocess_input_无法订阅感知输入*/
    EVENT_TYPE_APA_PPISIGHTINPUTTIMEOUT,               /*Apa_Parkingspace_Postprocess_input_视觉感知输入超时*/
    EVENT_TYPE_APA_PPIRADARINPUTABNORMAL,              /*Apa_Parkingspace_Postprocess_input_超声波雷达感知输入异常*/
    EVENT_TYPE_APA_PPIRADARSUBERROR = 182,             /*Apa_Parkingspace_Postprocess_input_无法订阅超声波雷达输入*/
    EVENT_TYPE_APA_PPIRADARINPUTTIMEOUT,               /*Apa_Parkingspace_Postprocess_input_超声波雷达感知输入超时*/
    EVENT_TYPE_APA_PPIVEHICLEINPUTABNORMAL,            /*Apa_Parkingspace_Postprocess_input_车身信号输入异常*/
    EVENT_TYPE_APA_PPIVEHICLESUBERROR,                 /*Apa_Parkingspace_Postprocess_input_无法订阅车身信号输入*/
    EVENT_TYPE_APA_PPIVEHICLEINPUTTIMEOUT,             /*Apa_Parkingspace_Postprocess_input_车身信号输入超时*/
    EVENT_TYPE_APA_PPSYSRUNTIMEOUT,                    /*Apa_Parkingspace_Postprocess_system_运行超时*/
    EVENT_TYPE_APA_PPODDSERROR,                        /*Apa_Parkingspace_Postprocess_output_发送失败*/
    EVENT_TYPE_APA_INFERINOBEVINPUT,                   /*Apa_Inference_input_没有鸟瞰图输入*/
    EVENT_TYPE_APA_INFERIBEVINPUTABNORMAL,             /*Apa_Inference_input_鸟瞰图输入异常*/
    EVENT_TYPE_APA_INFERSYSRUNTIMEOUT,                 /*Apa_Inference_system_运行超时*/
    EVENT_TYPE_APA_INFERPROCNNERROR = 192,             /*Apa_Inference_procedure_cnn处理错误*/
    EVENT_TYPE_APA_INFERPROFPSLOW,                     /*Apa_Inference_procedure_帧率过低*/
    EVENT_TYPE_APA_INFERODDSERROR,                     /*Apa_Inference_output_转化成dds消息失败*/
    EVENT_TYPE_APA_BEVICAMNOTDATA,                     /*Apa_Bev_input_获取不到四路鱼眼数据*/
    EVENT_TYPE_APA_BEVPROFPSLOW,                       /*Apa_Bev_procedure_帧率过低*/
    EVENT_TYPE_APA_BEVODDSERROR,                       /*Apa_Bev_output_转化成dds消息失败*/
    EVENT_TYPE_APA_CONILOCATIONTIMEOUT,                /*Apa_Control_input_获取定位数据超时*/
    EVENT_TYPE_APA_CONISUBLOCATIONDATAERROR,           /*Apa_Control_input_无法订阅定位数据*/
    EVENT_TYPE_APA_CONILOCATIONDATAERROR,              /*Apa_Control_input_定位数据异常*/
    EVENT_TYPE_APA_CONIVEHSIGTIMEOUT,                  /*Apa_Control_input_获取车身数据超时*/
    EVENT_TYPE_APA_CONIVEHSubDATAERROR = 202,          /*Apa_Control_input_无法订阅车身数据*/
    EVENT_TYPE_APA_CONIVEHSigDATAERROR,                /*车身数据信号异常*/
    EVENT_TYPE_APA_CONIPLANDATATIMEOUT,                /*Apa_Control_input_获取plan数据超时*/
    EVENT_TYPE_APA_CONIPLASubNDATAERROR,               /*Apa_Control_input_无法订阅plan数据*/
    EVENT_TYPE_APA_CONIPLANDATAERROR,                  /*Apa_Control_input_plan数据异常*/
    EVENT_TYPE_APA_ConSysRunTimeout,                   /*运行时间超过阈值*/
    EVENT_TYPE_APA_CONIProLBiasExceed,                 /*Apa_Control_procedure_横向偏差超出阈值*/
    EVENT_TYPE_APA_CONIProYawBiasExceed,               /*Apa_Control_procedure_航向偏差超出阈值*/
    EVENT_TYPE_APA_CONPROLATERALCONOVERTIME,           /*Apa_Control_procedure_横向控制解算时间超出阈值*/
    EVENT_TYPE_APA_CONOLIBFLOWERROR,                   /*Apa_Control_output_发送失败*/
    EVENT_TYPE_APA_CONODDSERROR = 212,                 /*Apa_Control_output_发送失败*/
    EVENT_TYPE_APA_PALNINOTSPOTS,                      /*Apa_Planning_input_无法获取车位信息*/
    EVENT_TYPE_APA_PALNIINVALIDSPOTS,                  /*Apa_Planning_input_无效的车位信息*/
    EVENT_TYPE_APA_PALNINOTVEHSIGNAL,                  /*Apa_Planning_input_无法获取车身数据*/
    EVENT_TYPE_APA_PALNINOTLOCATION,                   /*Apa_Planning_input_无法获取定位信息*/
    EVENT_TYPE_APA_PALNIINVALIDLOCATION,               /*Apa_Planning_input_invalid_location*/
    EVENT_TYPE_APA_PALNIINVALIDPARKMODE,               /*Apa_Planning_input_泊车类型异常*/
    EVENT_TYPE_APA_PALNSYSRUNTIMEOUT,                  /*Apa_Planning_system_运行时间超过阈值*/
    EVENT_TYPE_APA_PALNPROINVALIDSPOTCORNERS,          /*Apa_Planning_procedure_车位角点数据异常*/
    EVENT_TYPE_APA_PALNPROINVALIDPARKINGSTATUS,        /*Apa_Planning_procedure_无效的泊车状态*/
    EVENT_TYPE_APA_PALNODDSERROR = 222,                 /*Apa_Planning_output_发送失败*/
    EVENT_TYPE_APA_PALNOINVALIDTURNRADIUS,             /*Apa_Planning_output_转弯半径不合理*/
    EVENT_TYPE_APA_PALNONOPATH,                        /*Apa_Planning_output_没有路径输出*/
    EVENT_TYPE_APA_PALNOINVALIDPATHSIZE,              /*Apa_Planning_output_路径段数不合理*/
    EVENT_TYPE_APA_INFERPROACCERROR,                  /*Apa_Inference_procedure_tad4加速器错误*/
    EVENT_TYPE_APA_BEVCONFIGFILENOTEXIST,              /*Apa_Bev_confige_file不存在*/
    EVENT_TYPE_APA_BEVOPENVXERROR,                    /*Apa_Bev_openvx内部错误*/
    EVENT_TYPE_APA_LOCPROIMUINITERROR,                /*Apa_Location_procedure_imu_init_error_imu初始化失败*/
    EVENT_TYPE_APA_MAX = 230,
} EventType_t;

typedef struct EventData {
    EventType_t eventType;
    uint16_t eventStatus;
} EventData_t;

} // namespace EM

namespace FM {

typedef struct FaultData {
    uint32_t faultID;
    uint32_t faultObj;
    uint16_t faultStatus;
} FaultData_t;

} // namespace FM

namespace RM {

enum ProcessID {
    PROCESS_ID_MIN = 0,
    PROCESS_LANE,
    PROCESS_OBJECT_DETECT,
    PROCESS_FUSION,
    PROCESS_APA,
    PROCESS_PLANNING,

    PROCESS_ALGO_MAX = 10,

    PROCESS_CANOUT,
    PROCESS_CHASSIS,
    PROCESS_RADAR,
    PROCESS_PRIVATE_CANOUT,
    PROCESS_USS_RADAR,
    PROCESS_ANGLE_RADAR,
    PROCESS_ODO_VEHICLESIGNAL,
    PROCESS_CAN_MAX = 20,

    PROCESS_APP_FLEXIDAG_IMU,
    PROCESS_AVM_PARKING,
    PROCESS_APP_AVM_CAM_SERVER,
    PROCESS_APP_FRONT_CAM_SERVER,
    PROCESS_APP_CSITX,
    PROCESS_CALIBRATE,
    PROCESS_AVM_CALIB_SERVER,
    PROCESS_CALIB_CAN_SERVER,

    PROCESS_APP_MAX = 30,

    PROCESS_AVM_MANAGER,
    PROCESS_STATE_MANAGER,
    PROCESS_PHM_SERVER,
    PROCESS_FAULT_SERVER,
    PROCESS_SYS_SERVER,
    PROCESS_EVENT_SERVET,
    PROCESS_OTA_DOWNLOAD,
    PROCESS_APP_IPC_DIAG_SERVER,

    PROCESS_SERVER_MAX = 40,

    PROCESS_ID_MAX = 50,

    PROCESS_ALL
};

enum ModuleCommand { 
    MODULE_COMMAND_VERISON, 
    MODULE_COMMAND_MAX 
};

} // namespace RM
} // namespace minieye
