load("@rules_cc//cc:defs.bzl", "cc_library","cc_import")

cc_library(
    name = "libprotobuf",
    hdrs = glob(["include/**/*.h","include/**/*.inc"]),   
    strip_include_prefix = "include",
    deps = [":libprotobuf_private"],
#    srcs = ["lib/libprotobuf.so"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "libprotoc",
    hdrs = glob(["include/**/*.h","include/**/*.inc"]),   
    strip_include_prefix = "include",
    deps = [":libprotoc_private"],
#    src = ["lib/libprotoc.so"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "libprotobuf-lite",
    hdrs = glob(["include/**/*.h","include/**/*.inc"]),    
    strip_include_prefix = "include",
    deps = [":libprotobuf-lite_private"],
#    srcs = ["lib/libprotobuf-lite.so"],
    visibility = ["//visibility:public"],
)

cc_import(
  name = "libprotobuf_private",
  hdrs = glob(["include/**/**/*.h","include/**/**/**/*.h"]),
  shared_library = "lib/libprotobuf.so",
  visibility = ["//visibility:private"],
)

cc_import(
  name = "libprotoc_private",
  hdrs = glob(["include/**/**/*.h","include/**/**/**/*.h"]),
  shared_library = "lib/libprotoc.so",
  visibility = ["//visibility:private"],
)

cc_import(
  name = "libprotobuf-lite_private",
  hdrs = glob(["include/**/**/*.h","include/**/**/**/*.h"]),
  shared_library = "lib/libprotobuf-lite.so",
  visibility = ["//visibility:private"],
)