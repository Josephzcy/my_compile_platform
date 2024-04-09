load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "libassimp",
  srcs = glob([
        "lib/*.so*",
    ]),
  hdrs = glob(["include/assimp/*.h*",
              "include/assimp/*.h",
               "include/assimp/**/*.h*",
               "include/assimp/*.inl",
               ]),
  includes = ["include/",
              "include/assimp",
  ],
  visibility = ["//visibility:public"],
)
