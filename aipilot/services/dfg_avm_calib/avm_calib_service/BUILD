load("@rules_cc//cc:defs.bzl", "cc_library")
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "app_init",
    srcs = [
        "app_init.c",
    ],
    hdrs = [
        "app_init.h"
    ],
    deps = [
        "@bev_inf//:bev_inf",
    ],
)

cc_library(
    name = "avm_calib_service",
    srcs = [
        "avm_calib_service.cpp",
    ],
    hdrs = [
        "avm_calib_service.h"
    ],
    deps = [
        "@bev_inf//:bev_inf",
        "@libglog//:libglog",
        "@libdds//:libdds",
        "@libgtctimer//:libgtctimer",
        "@libDiagAgent//:libDiagAgent",
         "//aipilot/services/dfg_avm_calib/avm_calib_service:app_init",
        "//aipilot/services/dfg_avm_calib/avm_calib_service:WrapperDiag",
        "//aipilot/services/dfg_avm_calib/src:avm_calib",
        "//msg/precompiler:avm_calib_ctrl",

    ],
)

cc_library(
    name = "WrapperDiag",
    srcs = [
        "WrapperDiag.cpp",
    ],
    hdrs = [
        "WrapperDiag.h"
    ],
    deps = [
        "@bev_inf//:bev_inf",
        "@libglog//:libglog",
        "@libgtctimer//:libgtctimer",
        "@libDiagAgent//:libDiagAgent",
        "//aipilot/services/dfg_avm_calib/include:version",
    ],
)