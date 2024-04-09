load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "status_report",
    srcs = ['status_report.cc'],
    hdrs = ['status_report.h'],
    deps = [
        "@libglog_linux//:libglog_linux",
        "@libgflags_linux//:libgflags_linux"
    ],
    visibility = ["//visibility:public"]
)