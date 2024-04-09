load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "camera",
    hdrs = [
        "include/avm_camera/camera.h"
    ],
    deps = [
        "@libopencv4//:libopencv4",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "dfg_avm_mapping",
    srcs = ["avm_mapping.cpp",],
    hdrs = ["include/avm_mapping/avm_mapping.h"],
    includes = ["include"],
    deps = [
        "@libopencv4//:libopencv4",
        "@libglog//:libglog",
        "@com_github_json//:json",
        "//msg/precompiler:avm_calib_param",
        "//msg/precompiler:camera",
        "//aipilot/services/dfg_avm_mapping/src:avm_parse",
        "//aipilot/services/dfg_avm_mapping/src:camera_remapping",
    ],
    visibility = ["//visibility:public"],
)
