load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libApaCnnModule",
    hdrs = glob(["include/apa_cnn/.h"]),
    srcs = ["lib/libApaCnnModule.so"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkstatic = 0,    
)