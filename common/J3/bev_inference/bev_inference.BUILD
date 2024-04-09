load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "bev_inference",
    srcs = glob(["lib/**"]),    
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkstatic = 0,     
)


