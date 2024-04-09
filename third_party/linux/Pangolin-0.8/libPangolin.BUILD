load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libPangolin",
    hdrs = glob(["include/**/*.hpp"]),
    srcs = glob(["lib/*.so"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkstatic = 0,
)
