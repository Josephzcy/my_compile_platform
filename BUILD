load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")


filegroup(
  name = "pb",
  srcs = glob([
      "msg/precompiler/parking_gnss.pb.*",
      "msg/precompiler/parking_odometry.pb.*",
      "msg/precompiler/parkingspace.pb.*",
      "msg/precompiler/hmi_to_soc.pb.*",
      "msg/precompiler/imu.pb.*",
      "msg/precompiler/vehicle_signal.pb.*",
      "msg/precompiler/ap_map_response.pb.*",
      "msg/precompiler/ap_state.pb.*",
      "msg/precompiler/apa_state.pb.*",
      "msg/precompiler/map_engine_response.pb.*",
      "msg/precompiler/ap_map_status.pb.*",

      # 上面依赖的pb
      "msg/precompiler/geometry.pb.*",
      "msg/precompiler/data_source.pb.*",
      "msg/precompiler/data_header.pb.*",
      "msg/precompiler/planning.pb.*",    
    ]),
)

cc_library(
    name = "libap_map_tda4",
    srcs = glob(
        [
          "aipilot/modules/ap_slam/ap_map/common/**",
          "aipilot/modules/ap_slam/ap_map/config_manager/**",    
          "aipilot/modules/ap_slam/ap_map/data_transformer/**",
          "aipilot/modules/ap_slam/ap_map/src/**",       
        ], exclude = ["aipilot/modules/ap_slam/ap_map/config_manager/config/**"]) 
        + [":pb"],
    hdrs = glob([
          "msg/precompiler/*.h",
          "aipilot/modules/ap_slam/ap_map/src/*.h*",  
         # "aipilot/modules/ap_slam/ap_map/common/*.h*",                  
        ]),

    copts=[
        "-Imsg/precompiler",
        "-I aipilot/modules/ap_slam/ap_map/src",             
        "-Iaipilot/modules/ap_slam/ap_map",
        "-Iaipilot/modules/ap_slam/ap_map/config_manager",
        "-Iaipilot/modules/ap_slam/ap_map/data_transformer",
        "-std=c++17",
    ],
    deps = [
        "@com_gitlab_eigen3//:eigen",
        "@libyaml-cpp//:libyaml-cpp", 
        "@com_github_json//:json",
        "@libdds//:libdds",         
        "@libgtctimer//:libgtctimer",
        "@libglog//:libglog",
        "@libgflags//:libgflags",
        "@libprotobuffer//:libprotobuf",        
    ],      
    linkstatic = 0,
)

cc_binary(
    name = "ap_map_tda4",
    srcs = [
            "aipilot/modules/ap_slam/ap_map/ap_map.cpp",
            "aipilot/modules/ap_slam/ap_map/version.h"
          ],
    deps = [":libap_map_tda4" ],   
    copts=[
        "-Imsg/precompiler",
        "-I aipilot/modules/ap_slam/ap_map/src",             
        "-I aipilot/modules/ap_slam/ap_map",
        "-Iaipilot/modules/ap_slam/ap_map/common",
        "-std=c++17",
    ],     
    linkopts=["-pthread","-lm"],    
)