load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libflow",
    srcs = ["lib/libflow.so"],
    hdrs = glob(["include/*.hpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"], 
    linkstatic = 0, 
)