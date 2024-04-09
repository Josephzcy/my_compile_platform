load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
#load("//tools/install:install.bzl", "install", "install_files")
#load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "bazel_ex",
    srcs = [
        "bazel_ex.cc"
    ],
    deps=[
        "//examples/bazel_test/src/component:bazel_component",
    ],
    copts = ["-Iexamples/bazel_test/src/component"],
    linkopts = ["-pthread"],
)

#filegroup(
#    name = "srcs",
#    srcs = glob(["**"]),
#    visibility = ["//examples:__pkg__"],
#)