load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libnanomsg",
    hdrs = glob(["include/nanomsg/*.h*"]),
    srcs = ["lib/libnanomsg.so"],
    includes = ["include/"],     
    visibility = ["//visibility:public"],
    linkstatic = 0,    
)

