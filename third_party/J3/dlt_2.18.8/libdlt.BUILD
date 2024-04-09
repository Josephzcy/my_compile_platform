load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libdlt",
    hdrs = glob(["include/dlt/*.h"]),
    srcs = ["lib/libdlt.so"],  
    includes = ["include"],     
    visibility = ["//visibility:public"],
    linkstatic = 0, 
)
