load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "GtcAdasTime",
    hdrs = glob(["include/*.h"]),
    srcs = glob(["lib/libgtctimer.so*"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkstatic = 0,   
)