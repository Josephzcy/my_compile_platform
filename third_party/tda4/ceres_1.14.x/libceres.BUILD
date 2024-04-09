load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libceres",
    hdrs = glob(["include/ceres/*.h","include/ceres/internal/*.h"]),
    srcs = ["lib/libceres.so"],
    includes = ["include"],    
    visibility = ["//visibility:public"],
    linkstatic = 0,   
)