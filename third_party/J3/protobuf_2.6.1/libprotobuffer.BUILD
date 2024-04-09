load("@rules_cc//cc:defs.bzl", "cc_library","cc_import")

cc_library(
    name = "libprotobuf",
    hdrs = glob(["include/**/*.h","include/**/*.inc"]),   
    strip_include_prefix = "include",
    deps = [":libprotobuf_private"],
#    srcs = ["lib/libprotobuf.so"],
    visibility = ["//visibility:public"],
)

cc_import(
  name = "libprotobuf_private",
  hdrs = glob(["include/**/**/*.h","include/**/**/**/*.h"]),
  shared_library = "lib/libprotobuf.so",
  visibility = ["//visibility:private"],
)

