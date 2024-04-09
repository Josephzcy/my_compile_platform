load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libprotobuf",
    hdrs = glob(["include/**/*.h","include/**/*.inc"]),
    srcs = ["lib/libprotobuf.so"],
    strip_include_prefix = "include",    
    visibility = ["//visibility:public"],
)

cc_library(
    name = "libprotoc",
    hdrs = glob(["include/**/*.h","include/**/*.inc"]),
    srcs = ["lib/libprotoc.so"],
    strip_include_prefix = "include",    
    visibility = ["//visibility:public"],
)

cc_library(
    name = "libprotobuf-lite",
    hdrs = glob(["include/**/*.h","include/**/*.inc"]),
    srcs = ["lib/libprotobuf-lite.so"],
    strip_include_prefix = "include",    
    visibility = ["//visibility:public"],
)
