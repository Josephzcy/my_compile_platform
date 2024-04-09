load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libDiagAgent",
    hdrs = glob(["include/*.h"]),
    srcs = ["lib/libDiagAgent.so"],  
    includes = ["include"],     
    visibility = ["//visibility:public"],
    linkstatic = 0,  
)
