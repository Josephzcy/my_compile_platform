load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
#load("//tools/install:install.bzl", "install", "install_files")
#load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])
# BAZEL_TEST_COPTS = ['-DMODULE_NAME=\\"bazel_test\\"']

# 是否一个BUILD外放

#cc_binary(
#    name="bazel_test",
#    srcs = [
#        "test/bazel_test.cc",
#    ],
#    #hdrs = [
#    #    "bazel_test.h",
#    #],
#    deps =["//src:bazel_component"],
#)



#cc_library(
#    name="bazel_build_lib",
#    opts=BAZEL_TEST_COPTS,
#    deps=[
#        "@com_github_gflgs_gflags//:gflags",
#    ],
#)

#cc_library(
#    name = "bazel_component_lib",
#    srcs = ["bazel_component.cc"],
#    hdrs = ["bazel_component.h"],
#    copts = BAZEL_TEST_COPTS,
#    deps = [
#        ":bazel_build_lib",
#        "//examples/bazel_test/common:dependency_injector",
#    ],

#cc_binary(
#    name = "libbazel_component.so",
#    linkshared = True,
#    linkstatic = False,
#    deps = [":bazel_component_lib"],
#    )
#)

#install(
#    name = "install",
#    data = [
#        ":runtime_data",
#    ],
#       targets = [
#        ":libbazel_component.so",
#    ],
#    deps = [
#        ":pb_control",
#    ],
#)

#install_files(
#    name = "bazel_test",
#    dest = "examples/bazel_test",
#    files = [
#        "//modules/bazel/proto:pad_msg_py_pb2",
#    ],
#)

#filegroup(
#    name = "runtime_data",
#    srcs = glob([
#        "conf/*.txt",
#        "conf/*.conf",
#        "dag/*.dag",
#        "launch/*.launch",
#    ]),
#)

#cc_test(
#    name = "bazel_component_test",
#    size = "small",
#    srcs = ["bazel_component_test.cc"],
#    data = ["//examples/bazel_test:test_data"],
#    deps = [
#        ":bazel_component_lib",
#        "@com_google_googletest//:gtest_main",
#    ],
#)

# #TODO_1: split test_data
#filegroup(
#    name = "test_data",
#    srcs = glob([
#        "testdata/**",
#    ]),
#)

#cpplint()