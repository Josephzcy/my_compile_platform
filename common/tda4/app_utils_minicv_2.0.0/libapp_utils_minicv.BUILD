load("@rules_cc//cc:defs.bzl", "cc_library","cc_import")

cc_library(
    name = "libapp_utils_minicv",
    hdrs = [
        "include/minicv.h",       
    ],
    strip_include_prefix = "include",
    deps = [":libapp_utils_minicv_private"],
    visibility = ["//visibility:public"],
)

cc_import(
  name = "libapp_utils_minicv_private",
  hdrs = glob(["include/*.h"]),
  shared_library = "lib/libapp_utils_minicv.so",
  visibility = ["//visibility:private"],
)
