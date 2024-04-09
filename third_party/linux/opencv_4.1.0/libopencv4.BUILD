load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libopencv4",
    srcs = glob(["lib/libopencv*.so*"]),
    hdrs = glob(["include/opencv4/opencv2/**/*.h*"]),
    includes = ["include/opencv4/"],
    visibility = ["//visibility:public"], 
    linkstatic = 0, 
)
