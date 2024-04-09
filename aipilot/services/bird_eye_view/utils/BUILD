load("@rules_cc//cc:defs.bzl", "cc_library")
package(default_visibility = ["//visibility:public"])


cc_library(
    name = "common_utils",
    srcs = [
        "common_utils.cpp",
    ],
    hdrs = [
        "common_utils.h"
    ],
)

cc_library(
    name = "fps",
    hdrs = [
        "fps.h"
    ],
    deps = [
        "@libglog//:libglog",
    ],
)
