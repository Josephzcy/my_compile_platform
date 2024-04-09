load("@rules_cc//cc:defs.bzl", "cc_library")
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "jx65_layout",
    srcs = [
        "jx65_layout.cpp",
    ],
    hdrs = [
        "calibration_layout.h",
        "jx65_layout.h",
    ],
    deps = [
        "@com_github_json//:json",
        "@libglog//:libglog",
        "@libgflags//:libgflags",
        "//msg/precompiler:avm_calib_param",
        "@libopencv4//:libopencv4",
        "//msg/precompiler:camera",
        "//aipilot/services/dfg_avm_mapping:dfg_avm_mapping",
    ],
)

cc_library(
    name = "common_layout",
    srcs = [
        "common_layout.cpp",
    ],
    hdrs = [
        "calibration_layout.h",
        "common_layout.h"
    ],
    deps = [
        "@com_github_json//:json",
        "@libglog//:libglog",
        "@libgflags//:libgflags",
        "@libopencv4//:libopencv4",
        "//msg/precompiler:avm_calib_param",
        "//aipilot/services/dfg_avm_mapping:dfg_avm_mapping",
    ],
)

cc_library(
    name = "extrins_calib",
    srcs = [
        "extrins_calib.cpp",
    ],
    hdrs = [
        "extrins_calib.h",
        "calibration_layout.h",
        "common_layout.h"
    ],
    deps = [
        "@libglog//:libglog",
        "@libgflags//:libgflags",
        "@libopencv4//:libopencv4",
        "//msg/precompiler:avm_calib_param",
        "//msg/precompiler:camera",
        "//aipilot/services/dfg_avm_mapping:dfg_avm_mapping",
        "//aipilot/services/dfg_avm_calib/src:jx65_layout",
        "//aipilot/services/dfg_avm_calib/src:common_layout",
    ],
)

cc_library(
    name = "avm_calib",
    srcs = [
        "avm_calib.cpp",
    ],
    hdrs = [
        "avm_calib.h"
    ],
    deps = [
        "@libglog//:libglog",
        "//aipilot/services/dfg_avm_calib/src:extrins_calib",
        "//aipilot/services/dfg_avm_calib/src:jx65_layout",
        "//aipilot/services/dfg_avm_calib/src:common_layout",
        "//msg/precompiler:camera",
    ],
)