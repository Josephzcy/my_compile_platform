load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libdds",
    srcs = ["lib/libdds.so",
            "lib/libdds_api.so"],    
    hdrs = glob([
        "include/*.h",
        "include/*.hpp",      
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkstatic = 0,     
)


