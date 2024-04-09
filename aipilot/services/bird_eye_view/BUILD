load("@rules_cc//cc:defs.bzl", "cc_library")

cc_binary(
    name = "bird_eye_view",
    srcs = [
        "main_bev.cpp"
    ],
    deps = [
        "@libglog//:libglog",
        "//msg/precompiler:camera",
        "//aipilot/services/bird_eye_view/src:dsp_stitcher",
        "//aipilot/services/bird_eye_view/src:dds_processor",
        "//aipilot/services/bird_eye_view/src:global",
        "//aipilot/services/bird_eye_view/src:version",
        "//aipilot/services/bird_eye_view/src:wrapper_diag",
        "//aipilot/services/bird_eye_view/utils:common_utils",
        "//aipilot/services/bird_eye_view/utils:fps",
    ],
    copts = [
        "-DWL_EGL_PLATFORM",
    ],
    linkopts = ["-pthread"],
)
