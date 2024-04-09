load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "version",
    hdrs = ["version.h"],
    visibility = ["//visibility:public"]
)

cc_binary(
    name = "args_main",
    srcs = ['args_main.cc'],
    deps = [
        "@libglog_linux//:libglog_linux",
        "@libgflags_linux//:libgflags_linux",
        "//examples/args_main:version",
        "//examples/args_main/config:configuration",
        "//examples/args_main/src:status_report"
    ]
)