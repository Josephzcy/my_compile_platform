load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libyaml-cpp",
    hdrs = glob(["include/**"]),
    srcs = ["lib/libyaml-cpp.so"],  
    includes = ["include/"],     
    visibility = ["//visibility:public"],
    linkstatic = 0,   
)
