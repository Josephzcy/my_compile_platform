load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libglog",
    hdrs = glob(["include/glog/*.h"]),
    srcs = ["lib/libglog.so"],  
    includes = ["include"],  
    visibility = ["//visibility:public"],
)
