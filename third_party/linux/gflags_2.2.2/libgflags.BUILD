load("@rules_cc//cc:defs.bzl", "cc_library","cc_import")

cc_library(
    name = "libgflags",
    hdrs = glob(["include/gflags/*.h"]), 
    strip_include_prefix = "include",
    deps = [":libgflags_"],
    visibility = ["//visibility:public"],
)

cc_import(
  name = "libgflags_",
  hdrs = glob(["include/gflags/*.h"]),
  shared_library = "lib/libgflags.so",
  visibility = ["//visibility:private"],
)
