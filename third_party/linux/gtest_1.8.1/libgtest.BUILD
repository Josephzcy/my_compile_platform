load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libgtest",
    hdrs = glob(["include/**/*.h"]),
    srcs = ["lib/libgmock.so","lib/libgmock_main.so","lib/libgtest_main.so","lib/libgtest.so"],  
    includes = ["include"],     
    visibility = ["//visibility:public"],
    linkstatic = 0, 
)
