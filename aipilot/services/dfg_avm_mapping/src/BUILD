load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "avm_parse",
    srcs = [
        "avm_parse.cpp",
    ],
    hdrs = [
        "avm_parse.h"
    ],
    deps = [
        "@libopencv4//:libopencv4",
        "@com_github_json//:json",
        "@libglog//:libglog",
        "//msg/precompiler:avm_calib_param",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "camera_remapping",
    srcs = [
        "camera_remapping.cpp",
    ],
    hdrs = [
        "camera_remapping.h"
    ],
    deps = [
        "@libopencv4//:libopencv4",
        "@com_github_json//:json",
        "@libglog//:libglog",
        "//msg/precompiler:avm_calib_param",
        "//msg/precompiler:camera",
        "//aipilot/services/dfg_avm_mapping:camera",
        "//aipilot/services/dfg_avm_mapping/src:avm_parse",
        "//aipilot/services/dfg_avm_mapping/camera:cmei_model",
        "//aipilot/services/dfg_avm_mapping/camera:davide_scaramuzza_model",
        "//aipilot/services/dfg_avm_mapping/camera:fisheye_model",
    ],
    visibility = ["//visibility:public"],
)
