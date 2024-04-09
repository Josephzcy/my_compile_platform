load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
#load("//tools/install:install.bzl", "install", "install_files")
#load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name="bazel_component",
    # First Mod
    #srcs = [
    #    "bazel_component_inf.cc",
    #    "bazel_component.cc",
    #],
    #hdrs = glob([
    #    "bazel_component_inf.h",
    #    "bazel_component.h",
    #]),
    # Second Mod
    srcs = glob(
       ["*.cc"],
    ),
    hdrs = glob(
       ["*.h"],
    ),
    # copts = ["-I"],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)
