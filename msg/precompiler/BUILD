load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "apa_odometry",
    srcs = ['apa_odometry.pb.cc'],
    hdrs = ['apa_odometry.pb.h'],
    deps=["@libprotobuffer//:libprotobuf",
    	  ":geometry",],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "apa_state",
    srcs = ['apa_state.pb.cc'],
    hdrs = ['apa_state.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "avm_calib_ctrl",
    srcs = ['avm_calib_ctrl.pb.cc'],
    hdrs = ['avm_calib_ctrl.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "avm_calib_param",
    srcs = ['avm_calib_param.pb.cc'],
    hdrs = ['avm_calib_param.pb.h'],
    deps=["@libprotobuffer//:libprotobuf",
    	  ":camera",],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "calib_param",
    srcs = ['calib_param.pb.cc'],
    hdrs = ['calib_param.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "camera_occlusion",
    srcs = ['camera_occlusion.pb.cc'],
    hdrs = ['camera_occlusion.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "camera",
    srcs = ['camera.pb.cc'],
    hdrs = ['camera.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "cipv",
    srcs = ['cipv.pb.cc'],
    hdrs = ['cipv.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "data_header",
    srcs = ['data_header.pb.cc'],
    hdrs = ['data_header.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "data_source",
    srcs = ['data_source.pb.cc'],
    hdrs = ['data_source.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "dvr",
    srcs = ['dvr.pb.cc'],
    hdrs = ['dvr.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "fail_detection",
    srcs = ['fail_detection.pb.cc'],
    hdrs = ['fail_detection.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "freespace_obstacles",
    srcs = ['freespace_obstacles.pb.cc'],
    hdrs = ['freespace_obstacles.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "freespacepoints",
    srcs = ['freespacepoints.pb.cc'],
    hdrs = ['freespacepoints.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "geometry",
    srcs = ['geometry.pb.cc'],
    hdrs = ['geometry.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "gnss",
    srcs = ['gnss.pb.cc'],
    hdrs = ['gnss.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "haf_location",
    srcs = ['haf_location.pb.cc'],
    hdrs = ['haf_location.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "hmi_to_soc",
    srcs = ['hmi_to_soc.pb.cc'],
    hdrs = ['hmi_to_soc.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "ihu_to_soc",
    srcs = ['ihu_to_soc.pb.cc'],
    hdrs = ['ihu_to_soc.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "ihc",
    srcs = ['ihc.pb.cc'],
    hdrs = ['ihc.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "imu",
    srcs = ['imu.pb.cc'],
    hdrs = ['imu.pb.h'],
    deps = [
        "@libprotobuffer//:libprotobuf",
        ":data_header",],     
    visibility = ["//visibility:public"]
)

cc_library(
    name = "ins",
    srcs = ['ins.pb.cc'],
    hdrs = ['ins.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "lidar",
    srcs = ['lidar.pb.cc'],
    hdrs = ['lidar.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "location_common",
    srcs = ['location_common.pb.cc'],
    hdrs = ['location_common.pb.h'],
    deps=["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "object_warning",
    srcs = ['object_warning.pb.cc'],
    hdrs = ['object_warning.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],    
    visibility = ["//visibility:public"]
)

cc_library(
    name = "object",
    srcs = ['object.pb.cc'],
    hdrs = ['object.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],  
    visibility = ["//visibility:public"]
)

cc_library(
    name = "odo_vehicle_signal",
    srcs = ['odo_vehicle_signal.pb.cc'],
    hdrs = ['odo_vehicle_signal.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],   
    visibility = ["//visibility:public"]
)

cc_library(
    name = "odometry",
    srcs = ['odometry.pb.cc'],
    hdrs = ['odometry.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "odometry_3d",
    srcs = ['odometry_3d.pb.cc'],
    hdrs = ['odometry_3d.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "parkingspace",
    srcs = ['parkingspace.pb.cc'],
    hdrs = ['parkingspace.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf",
     	    ":geometry",
     	    ":data_source",],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "pedestrian",
    srcs = ['pedestrian.pb.cc'],
    hdrs = ['pedestrian.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "planning",
    srcs = ['planning.pb.cc'],
    hdrs = ['planning.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "planningtohmi",
    srcs = ['planningtohmi.pb.cc'],
    hdrs = ['planningtohmi.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "prediction",
    srcs = ['prediction.pb.cc'],
    hdrs = ['prediction.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "RawInsParkingSpace",
    srcs = ['RawInsParkingSpace.pb.cc'],
    hdrs = ['RawInsParkingSpace.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "road_seg",
    srcs = ['road_seg.pb.cc'],
    hdrs = ['road_seg.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "roadmarking_hz",
    srcs = ['roadmarking_hz.pb.cc'],
    hdrs = ['roadmarking_hz.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "scene",
    srcs = ['scene.pb.cc'],
    hdrs = ['scene.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "segmentation",
    srcs = ['segmentation.pb.cc'],
    hdrs = ['segmentation.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "tag",
    srcs = ['tag.pb.cc'],
    hdrs = ['tag.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "tsr",
    srcs = ['tsr.pb.cc'],
    hdrs = ['tsr.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "ultra_radar",
    srcs = ['ultra_radar.pb.cc'],
    hdrs = ['ultra_radar.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "vehicle_control",
    srcs = ['vehicle_control.pb.cc'],
    hdrs = ['vehicle_control.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "vehicle_signal",
    srcs = ['vehicle_signal.pb.cc'],
    hdrs = ['vehicle_signal.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "vehicle",
    srcs = ['vehicle.pb.cc'],
    hdrs = ['vehicle.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "apa_gnss",
    srcs = ['apa_gnss.pb.cc'],
    hdrs = ['apa_gnss.pb.h'],
    deps = [
        "@libprotobuffer//:libprotobuf",
        ":data_header",],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "vtr",
    srcs = ['vtr.pb.cc'],
    hdrs = ['vtr.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "parking_mask",
    srcs = ['parking_mask.pb.cc'],
    hdrs = ['parking_mask.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "ap_map_response",
    srcs = ['ap_map_response.pb.cc'],
    hdrs = ['ap_map_response.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "ap_map_status",
    srcs = ['ap_map_status.pb.cc'],
    hdrs = ['ap_map_status.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "parking_gnss",
    srcs = ['parking_gnss.pb.cc'],
    hdrs = ['parking_gnss.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "parking_odometry",
    srcs = ['parking_odometry.pb.cc'],
    hdrs = ['parking_odometry.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "parking_ins",
    srcs = ['parking_ins.pb.cc'],
    hdrs = ['parking_ins.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "map_engine_response",
    srcs = ['map_engine_response.pb.cc'],
    hdrs = ['map_engine_response.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf", 
            ":planning",
            ":odometry_3d",
            ":parkingspace"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "roadmarks",
    srcs = ['roadmarks.pb.cc'],
    hdrs = ['roadmarks.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf",
            ":geometry"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "soc_to_ihu",
    srcs = ['soc_to_ihu.pb.cc'],
    hdrs = ['soc_to_ihu.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "parking_object",
    srcs = ['parking_object.pb.cc'],
    hdrs = ['parking_object.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "apa_hmi_signal",
    srcs = ['apa_hmi_signal.pb.cc'],
    hdrs = ['apa_hmi_signal.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "camera_shelter",
    srcs = ['camera_shelter.pb.cc'],
    hdrs = ['camera_shelter.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)

cc_library(
    name = "uss_output",
    srcs = ['uss_output.pb.cc'],
    hdrs = ['uss_output.pb.h'],
    deps = ["@libprotobuffer//:libprotobuf"],      
    visibility = ["//visibility:public"]
)
