load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libApaCommon",
    hdrs = glob(["include/**/*.h"]),
    srcs = ["lib/libApaCommon.so"],
    strip_include_prefix = "include",    
    visibility = ["//visibility:public"],
)
