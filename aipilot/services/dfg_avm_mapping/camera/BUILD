load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "brown_model",
    srcs = [
        "brown_model.cpp",
    ],
    hdrs = [
        "brown_model.h"
    ],
    deps = [
        "@libglog//:libglog",
        "//aipilot/services/dfg_avm_mapping:camera",
        "//aipilot/services/dfg_avm_mapping/camera:pinhole_model",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cmei_model",
    srcs = [
        "cmei_model.cpp",
    ],
    hdrs = [
        "cmei_model.h"
    ],
    deps = [
        "@libopencv4//:libopencv4",
        "@libglog//:libglog",
        "//aipilot/services/dfg_avm_mapping:camera",
        "//aipilot/services/dfg_avm_mapping/camera:pinhole_model",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "davide_scaramuzza_model",
    srcs = [
        "davide_scaramuzza_model.cpp",
    ],
    hdrs = [
        "davide_scaramuzza_model.h"
    ],
    deps = [
        "@libopencv4//:libopencv4",
        "@libglog//:libglog",
        "//aipilot/services/dfg_avm_mapping:camera",
        "//aipilot/services/dfg_avm_mapping/camera:pinhole_model",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "fisheye_model",
    srcs = [
        "fisheye_model.cpp",
    ],
    hdrs = [
        "fisheye_model.h"
    ],
    deps = [
        "@libopencv4//:libopencv4",
        "@libglog//:libglog",
        "//aipilot/services/dfg_avm_mapping:camera",
        "//aipilot/services/dfg_avm_mapping/camera:pinhole_model",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "pinhole_model",
    srcs = [
        "pinhole_model.cpp",
    ],
    hdrs = [
        "pinhole_model.h"
    ],
    deps = [
        "@libopencv4//:libopencv4",
        "@libglog//:libglog",
        "//aipilot/services/dfg_avm_mapping:camera",
    ],
    visibility = ["//visibility:public"],
)