workspace(name = "com_minieye_aiplorer")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

load(":deps.bzl", "toolchains")
toolchains()

load("//bazel/toolchain:toolchain.bzl", "register_all_toolchains")
register_all_toolchains()

load("//:.repo.bzl", "PLATFORM") 

#json header lib fo linux & tda4 & j3
new_local_repository(
    name = "json",
    path = "third_party/json_3.10.5",
    build_file = "//third_party/json_3.10.5:json.BUILD"
)
#json 兼容引用com_github_json,跟上面是同一版本json库
http_archive(
    name = "com_github_json",
    build_file = "//third_party/tda4:json.BUILD",
    sha256 = "5daca6ca216495edf89d167f808d1d03c4a4d929cef7da5e10f135ae1540c7e4",
    strip_prefix = "json-3.10.5",
    urls = ["https://github.com/nlohmann/json/archive/refs/tags/v3.10.5.tar.gz"],
)
#new_local_repository(
#    name = "com_github_json",
#    path = "third_party/tda4/json_3.10.5",
#    build_file_content =
#"""
#cc_library(
#    name = 'json',
#    hdrs = glob(['include/json.hpp']),       
#    srcs = [],
#    includes = ['include'],
#    visibility = ['//visibility:public'],
#)
#"""
#)

new_local_repository(
    name = "interfaceHeader",
    path = "common/"+ PLATFORM +"/apa_interface_1.0.0/include",
    build_file = "//common/"+ PLATFORM +"/apa_interface_1.0.0/include:interfaceHeader.BUILD"
)

#spdlog for linux & tda4 & j3
new_local_repository(
    name = "spdlog",
    path = "third_party/spdlog_1.10.0",
    build_file = "//third_party/spdlog_1.10.0:spdlog.BUILD",
)

#msgpack header lib for linux & tda4 & j3
new_local_repository(
    name = "msgpack",
    path = "third_party/msgpack-c_3.1.1/include",
#    build_file = "//third_party/msgpack-c_3.1.1/include:msgpack.BUILD",
    build_file_content =
"""
cc_library(
    name = 'msgpack',
    hdrs = glob(['*.h*','msgpack/*.h*','msgpack/**/*.h*']),    
    srcs = [],
    includes = ['.'],
    visibility = ['//visibility:public'],
)
"""
)

#http_archive(
#    name = "com_github_msgpack",
#    build_file = "//third_party/tda4:msgpackHeader.BUILD",
#    sha256 = "bda49f996a73d2c6080ff0523e7b535917cd28c8a79c3a5da54fc29332d61d1e",
#    strip_prefix = "msgpack-c-cpp-3.1.1",
#    urls = ["https://github.com/msgpack/msgpack-c/archive/refs/tags/cpp-3.1.1.tar.gz",
#            "http://192.168.66.113:8092/msgpack-c-cpp-3.1.1.tar.gz"],
#)

#eigen 3.3.4 for linux & tda4 & j3
new_local_repository(
    name = "eigen",
    path = "third_party/eigen_3.3.4/include",
    build_file_content =
"""
cc_library(
    name = 'eigen',
    srcs = [],
    includes = ['.'],
    hdrs = glob(['Eigen/**','unsupported/Eigen/**']),
    visibility = ['//visibility:public'],
)
"""
)

new_local_repository(
    name = "com_gitlab_eigen3",
    path = "third_party/eigen_3.4.0/include",
    build_file_content =
"""
cc_library(
    name = 'eigen',
    srcs = [],
    includes = ['.'],
    hdrs = glob(['Eigen/**','unsupported/Eigen/**']),
    visibility = ['//visibility:public'],
)
"""
)

# Eigen 3.4.0 for high version compiler
#http_archive(
#    name = "com_gitlab_eigen3",
#    sha256 = "b4c198460eba6f28d34894e3a5710998818515104d6e74e5cc331ce31e46e626",
#    strip_prefix = "eigen-3.4.0",
#    urls = ["https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.bz2"],
#    build_file_content =
#)

#opencv4.1.0 for linux & tda4 & j3
new_local_repository(
    name = "libopencv4",
    path = "third_party/"+ PLATFORM +"/opencv_4.1.0",
    build_file = "//third_party/"+ PLATFORM +"/opencv_4.1.0:libopencv4.BUILD",
)
#new_local_repository(
#    name = "libopencv4",
#    path = "third_party/linux/opencv_4.1.0",
#    build_file = "//third_party/linux/opencv_4.1.0:libopencv4_linux.BUILD",
#)

#opencv3.4.8 for linux & tda4 & j3
new_local_repository(
    name = "libopencv3",
    path = "third_party/"+ PLATFORM +"/opencv_3.4.8",
    build_file = "//third_party/"+ PLATFORM +"/opencv_3.4.8:libopencv3.BUILD",
)

#libdds for linux & tda4 & j3
new_local_repository(
    name = "libdds",
    path = "common/"+ PLATFORM +"/dds",
    build_file = "//common/"+ PLATFORM +"/dds:libdds.BUILD"
)

#GtcAdasTime for linux only
new_local_repository(
    name = "GtcAdasTime",
    path = "common/linux/GtcAdasTime_1.0.0",
    build_file = "//common/linux/GtcAdasTime_1.0.0:GtcAdasTime.BUILD"
)
#libgtctimer for linux & tda4 & j3
new_local_repository(
    name = "libgtctimer",
    path = "common/"+ PLATFORM +"/gtctimer_0.1.2",
    build_file = "//common/"+ PLATFORM +"/gtctimer_0.1.2:libgtctimer.BUILD"
)

#libosqp for tda4 & linux
new_local_repository(
    name = "libosqp",            #osqp -> libosqp
    path = "third_party/"+ PLATFORM +"/osqp_0.5.0",
    build_file = "//third_party/"+ PLATFORM +"/osqp_0.5.0:libosqp.BUILD"
)
#new_local_repository(
#    name = "libosqp",
#    path = "third_party/tda4/osqp_0.5.0",
#    build_file = "//third_party/tda4/osqp_0.5.0:libosqp.BUILD"
#)

#bev & inference for tda4 version
new_local_repository(
    name = "bev_inference",
    path = "common/tda4/bev_inference",
    build_file = "//common/tda4/bev_inference:bev_inference.BUILD"
)
new_local_repository(
    name = "bev_inf",
    path = "common/tda4/sysroots",
    build_file = "//common/tda4/sysroots:bev_inf.BUILD"
)

#libglm for linux & tda4 & j3
new_local_repository(
    name = "libglm",
    path = "third_party/glm-0.9.9.8",
    build_file = "//third_party/glm-0.9.9.8:libglm.BUILD"
)

#no use, to be del ...
#new_local_repository(
#    name = "librpc",
#    path = "third_party/"+ PLATFORM +"/rpc_1.0.1",
#    build_file = "//third_party/"+ PLATFORM +"/rpc_1.0.1:librpc.BUILD"
#)
#new_local_repository(
#    name = "librpc",
#    path = "third_party/linux/rpc_1.0.1",
#    build_file = "//third_party/linux/rpc_1.0.1:librpc.BUILD"
#)

#libapp_utils_minicv for tda4 only
new_local_repository(
    name = "libapp_utils_minicv",
    path = "common/tda4/app_utils_minicv_2.0.0",
    build_file = "//common/tda4/app_utils_minicv_2.0.0:libapp_utils_minicv.BUILD"
)

#libApaCommon for tda4 & linux, maybe no use, to be deleted...
new_local_repository(
    name = "libApaCommon",
    path = "common/"+ PLATFORM +"/apa_common_1.0.0",
    build_file = "//common/"+ PLATFORM +"/apa_common_1.0.0:libApaCommon.BUILD"
)
#new_local_repository(
#    name = "libApaCommon_linux",
#    path = "common/linux/apa_common_1.0.0",
#    build_file = "//common/linux/apa_common_1.0.0:libApaCommon.BUILD"
#)

#libcommon for tda4 & linux & J3
new_local_repository(
    name = "libcommon",
    path = "common/"+ PLATFORM +"/common_2.0.1",
    build_file = "//common/"+ PLATFORM +"/common_2.0.1:libcommon.BUILD"
)
#new_local_repository(
#    name = "libcommon_linux",
#    path = "common/linux/common_2.0.1",
#    build_file = "//common/linux/common_2.0.1:libcommon.BUILD"
#)

# libprotobuffer for linux & tda4 & j3
new_local_repository(
    name = "libprotobuffer",
    path = "third_party/"+ PLATFORM +"/protobuf_3.12.3",
    build_file = "//third_party/"+ PLATFORM +"/protobuf_3.12.3:libprotobuffer.BUILD"
)
#new_local_repository(
#    name = "libprotobuffer_linux",
#    path = "third_party/linux/protobuf_3.12.3",
#    build_file = "//third_party/linux/protobuf_3.12.3:libprotobuffer.BUILD"
#)

# libgflags for linux & tda4 & j3
new_local_repository(
    name = "libgflags",
    path = "third_party/"+ PLATFORM +"/gflags_2.2.2",
    build_file = "//third_party/"+ PLATFORM +"/gflags_2.2.2:libgflags.BUILD"
)
#new_local_repository(
#    name = "libgflags",
#    path = "third_party/linux/gflags_2.2.2",
#    build_file = "//third_party/linux/gflags_2.2.2:libgflags.BUILD"
#)

#libflow for linux & tda4 & j3
new_local_repository(
    name = "libflow",
    path = "third_party/"+ PLATFORM +"/libflow_1.0.0",
    build_file = "//third_party/"+ PLATFORM +"/libflow_1.0.0:libflow.BUILD"
)
#new_local_repository(
#    name = "libflow_linux",
#    path = "third_party/linux/libflow_1.0.21.4.10",
#    build_file = "//third_party/linux/libflow_1.0.0:libflow.BUILD"
#)

#libglog for tda4 & linux & J3
new_local_repository(
    name = "libglog",
    path = "third_party/"+ PLATFORM +"/glog_0.3.5",
    build_file = "//third_party/"+ PLATFORM +"/glog_0.3.5:libglog.BUILD"
)
#new_local_repository(
#    name = "libglog_linux",
#    path = "third_party/linux/glog_0.3.5-rc.5",
#    build_file = "//third_party/linux/glog_0.3.5:libglog.BUILD"
#)

# libceres for linux & tda4 & j3
new_local_repository(
    name = "libceres",
    path = "third_party/"+ PLATFORM +"/ceres_1.14.x",
    build_file = "//third_party/"+ PLATFORM +"/ceres_1.14.x:libceres.BUILD"
)
#new_local_repository(
#    name = "libceres_linux",
#    path = "third_party/linux/ceres_1.14.x",
#    build_file = "//third_party/linux/ceres_1.14.x:libceres.BUILD"
#)

#libnanomsg for linux & tda4 & j3
new_local_repository(
    name = "libnanomsg",
    path = "third_party/"+ PLATFORM +"/nanomsg_1.1.5",
    build_file = "//third_party/"+ PLATFORM +"/nanomsg_1.1.5:libnanomsg.BUILD"
)
#new_local_repository(
#    name = "libnanomsg_linux",
#    path = "third_party/linux/nanomsg_1.1.5",
#    build_file = "//third_party/linux/nanomsg_1.1.5:libnanomsg.BUILD"
#)

#libopenblas only linux version
new_local_repository(
    name = "libopenblas",
    path = "third_party/linux/openblas_0.3.5-rc.3",
    build_file = "//third_party/linux/openblas_0.3.5-rc.3:libopenblas.BUILD"
)

#libyaml-cpp for linux & tda4 & j3
new_local_repository(
    name = "libyaml-cpp",
    path = "third_party/"+ PLATFORM +"/yaml-cpp-0.7.0",
    build_file = "//third_party/"+ PLATFORM +"/yaml-cpp-0.7.0:libyaml-cpp.BUILD"
)
#new_local_repository(
#    name = "libyaml-cpp_linux",
#    path = "third_party/linux/yaml-cpp-0.7.0",
#    build_file = "//third_party/linux/yaml-cpp-0.7.0:libyaml-cpp_linux.BUILD"
#)

#libDiagAgent for linux & tda4 & j3, note that diagnosis feature not work for linux & j3, just used for building
new_local_repository(
    name = "libDiagAgent",
    path = "common/"+ PLATFORM +"/diag_agent_1.0.11",
    build_file = "//common/"+ PLATFORM +"/diag_agent_1.0.11:libDiagAgent.BUILD"
)
#new_local_repository(
#    name = "libDiagAgent_linux",
#    path = "common/linux/diag_agent_1.0.11",
#    build_file = "//common/linux/diag_agent_1.0.11:libDiagAgent.BUILD"
#)

#libdlt for linux & tda4 & j3
new_local_repository(
    name = "libdlt",
    path = "third_party/"+ PLATFORM +"/dlt_2.18.8",
    build_file = "//third_party/"+ PLATFORM +"/dlt_2.18.8:libdlt.BUILD"
)

#libgtest for linux only
new_local_repository(
    name = "libgtest",
    path = "third_party/linux/gtest_1.8.1",
    build_file = "//third_party/linux/gtest_1.8.1:libgtest.BUILD"
)

#libtda4_accelerator for tda4 only
new_local_repository(
    name = "libtda4_accelerator",
    path = "common/tda4/tda4_accelerator_1.0.2",
    build_file = "//common/tda4/tda4_accelerator_1.0.2:libtda4_accelerator.BUILD"
)

#libassimp for linux & tda4
new_local_repository(
    name = "libassimp",
    path = "third_party/"+ PLATFORM +"/assimp_5.2.2",
    build_file = "//third_party/"+ PLATFORM +"/assimp_5.2.2:libassimp.BUILD"
)

#libApaTransparent for tda4
new_local_repository(
    name = "libApaTransparent",
    path = "common/tda4/apa_transparent",
    build_file = "//common/tda4/apa_transparent:libApaTransparent.BUILD"
)

#########     J3平台库     #########
#libextraflow for J3 & linux
new_local_repository(
    name = "libextraflow",
    path = "common/"+ PLATFORM +"/extraflow",
    build_file = "//common/"+ PLATFORM +"/extraflow:libextraflow.BUILD"
)
#new_local_repository(
#    name = "libextraflow",
#    path = "common/linux/extraflow",
#    build_file = "//common/linux/extraflow:libextraflow.BUILD"
#)

#libobj_cnn for j3 only
new_local_repository(
    name = "libobj_cnn",
    path = "common/J3/obj_cnn",
    build_file = "//common/J3/obj_cnn:libobj_cnn.BUILD"
)

#libPangolin for linux only
#new_local_repository(
#    name = "libPangolin",
#    path = "third_party/linux/Pangolin-0.8",
#    build_file = "//third_party/linux/Pangolin-0.8:libPangolin.BUILD"
#)

#new_local_repository(
#    name = "libprotobuffer",        # libprotobuf_J3 -> libprotobuffer
#    path = "third_party/J3/protobuf_3.12.3",
#    build_file = "//third_party/J3/protobuf_3.12.3:libprotobuffer.BUILD"
#)
