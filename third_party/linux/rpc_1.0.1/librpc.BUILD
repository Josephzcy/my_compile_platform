load("@rules_cc//cc:defs.bzl", "cc_library","cc_import")

cc_library(
    name = "librpc",
    hdrs = glob(["include/**/*.h*"]),
    strip_include_prefix = "include",
    deps = [":librpc_private"],
    visibility = ["//visibility:public"],
)

cc_import(
  name = "librpc_private",
  hdrs = glob(["include/rpc/*.h", "include/rpc/detail/*.h"]),
  shared_library = "lib/librpc.so",  
  visibility = ["//visibility:private"],
)


