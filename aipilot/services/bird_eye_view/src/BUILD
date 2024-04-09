load("@rules_cc//cc:defs.bzl", "cc_library")
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "dds_processor",
    srcs = [
        "dds_processor.cpp",
    ],
    hdrs = [
        "dds_processor.h"
    ],
    deps = [
        "@libdds//:libdds",
        "@libglog//:libglog",
        "//msg/precompiler:apa_state",
        "//msg/precompiler:camera",
        "//msg/precompiler:vehicle_signal",
        "//aipilot/services/bird_eye_view/src:global",
        "//aipilot/services/bird_eye_view/src:wrapper_diag",
    ],
)

cc_library(
    name = "dsp_stitcher",
    srcs = [
        "dsp_stitcher.cpp",
    ],
    hdrs = [
        "dsp_stitcher.h"
    ],
    deps = [
        "@libopencv4//:libopencv4",
        "@libapp_utils_minicv//:libapp_utils_minicv",
        "@com_github_json//:json",
        "@bev_inference//:bev_inference",
        "@bev_inf//:bev_inf",
        "@libglog//:libglog",
        "//msg/precompiler:camera",
        "//aipilot/services/bird_eye_view/src:global",
    ],
)

cc_library(
    name = "global",
    srcs = [
        "global.cpp",
    ],
    hdrs = [
        "global.h"
    ],
    deps = [
        "@libgflags//:libgflags",
    ],
)

cc_library(
    name = "version",
    hdrs = [
        "version.h"
    ],
)

cc_library(
    name = "wrapper_diag",
    srcs = [
        "wrapper_diag.cpp",
    ],
    hdrs = [
        "wrapper_diag.h"
    ],
    deps = [
        "@libglog//:libglog",
        "@libgtctimer//:libgtctimer",
        "@libDiagAgent//:libDiagAgent",
    ],
)
