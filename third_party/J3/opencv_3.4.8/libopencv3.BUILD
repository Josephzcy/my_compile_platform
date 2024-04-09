load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libopencv3",
    srcs = glob(["lib/libopencv*.so*"]),
    hdrs = glob(["include/opencv/**/*.h*","include/opencv2/**/*.h*"]),
    includes = ["include/"],
    visibility = ["//visibility:public"], 
    linkstatic = 0, 
)
