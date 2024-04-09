load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libobj_cnn",
    hdrs = glob([
        "include/obj_cnn/*.h",
        "include/obj_cnn/operators/*.h",
        "include/toml/*.hpp",
        "include/toml.hpp",
        "include/simplelog/*.h",
        "include/logfacade/*.h",
        "include/minieye_jsonhpp/*.hpp",
    ]),
    srcs = glob([
        "lib/*.so",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkstatic = 0,    
)
