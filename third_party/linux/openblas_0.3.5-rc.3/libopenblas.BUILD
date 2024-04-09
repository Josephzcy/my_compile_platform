load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libopenblas",
    hdrs = glob(["include/openblas/*.h*"]),
    srcs = ["lib/libopenblas.so"],
    includes = ["include/"],     
    visibility = ["//visibility:public"],
    linkstatic = 0,    
)

