load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "adol-c",
    hdrs = glob(["include/**/*.h"]),
    srcs = glob(["lib/*.so*"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkstatic = 0,   
)