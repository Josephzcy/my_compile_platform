load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libextraflow",
    srcs = ["lib/libextraflow.so"],    
    hdrs = glob([
        "include/extraflow/*.h"
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkstatic = 0,     
)


