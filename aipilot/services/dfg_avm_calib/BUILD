load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "dfg_avm_calib",
    srcs = [
        "avm_calib_task.cpp",
    ],
    deps = [
        "//aipilot/services/dfg_avm_calib/avm_calib_service:app_init",
        "//aipilot/services/dfg_avm_calib/avm_calib_service:avm_calib_service",
        "//aipilot/services/dfg_avm_calib/avm_calib_service:WrapperDiag",
        "//aipilot/services/dfg_avm_calib/src:avm_calib",
        "//aipilot/services/dfg_avm_mapping:dfg_avm_mapping",
    ],
    copts = [
        "-Iaipilot/services/dfg_avm_calib",
        "-Iaipilot/services/dfg_avm_mapping",
    ],
    linkopts = ["-pthread"],
)
