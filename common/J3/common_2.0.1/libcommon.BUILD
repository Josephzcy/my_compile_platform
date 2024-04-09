load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libcommon",
    hdrs = glob(["include/common/**/*.h*","include/common/**/**/*.h*"]), 
    srcs = ["lib/libcommon.so"],
    strip_include_prefix = "include",    
    visibility = ["//visibility:public"],
)