load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

cc_test(
    name = "bazel_test",
    srcs = [
      "bazel_test.cc",
    ],
    copts = ["-Iexamples/bazel_test/src/component"],
    deps = [
      "@com_google_googletest_1.10.0//:gtest_main",
      "//examples/bazel_test/src/component:bazel_component",
    ],
)