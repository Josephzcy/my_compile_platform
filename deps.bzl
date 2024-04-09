load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#URL_TOOLCHAIN = "http://192.168.66.113:8092/"
#URL_SYSROOT = "http://192.168.66.113:8092/"


def toolchains():
    if "aarch64-none-linux-gnu" not in native.existing_rules():
        native.new_local_repository(
            name = "aarch64-none-linux-gnu",
            path = "/opt/toolchains/tda4/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu",        
            build_file = "//bazel/toolchain/aarch64-none-linux-gnu:aarch64-none-linux-gnu.BUILD",
            #url = URL_TOOLCHAIN + "aarch64-none-linux-gnu.tar.gz",
            #sha256 = "2439bb4f1345e3e82812aeeb5c86bd16e01486daab7b705b78852dd8802d8de0",
        )

    if "aarch64-none-linux-gnu-sysroot" not in native.existing_rules():
        native.new_local_repository(
            name = "aarch64-none-linux-gnu-sysroot",
            path = "/opt/toolchains/tda4/sysroots",
            build_file = "//bazel/toolchain/aarch64-none-linux-gnu:aarch64-none-linux-gnu-sysroot.BUILD",
            #url = URL_SYSROOT + "aarch64-none-linux-gnu-sysroot.tar.gz",
            #sha256 = "20e0ca87176fed5ac3b2ab05856e287a7683829a0753b5d29d3d540f04b79f96",
        )  

    if "aarch64-j3-linux-gnu" not in native.existing_rules():
        native.new_local_repository(
            name = "aarch64-j3-linux-gnu",
            path = "/opt/toolchains/J3/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu",        
            build_file = "//bazel/toolchain/aarch64-j3-linux-gnu:aarch64-j3-linux-gnu.BUILD",
        )

    if "aarch64-j3-linux-gnu-sysroot" not in native.existing_rules():
        native.new_local_repository(
            name = "aarch64-j3-linux-gnu-sysroot",
            path = "/opt/toolchains/J3/sysroots",
            build_file = "//bazel/toolchain/aarch64-j3-linux-gnu:aarch64-j3-linux-gnu-sysroot.BUILD",
        )

#    if "arm-cortex_a8-linux-gnueabihf" not in native.existing_rules():
#        http_archive(
#            name = "arm-cortex_a8-linux-gnueabihf",
#            build_file = Label("//bazel/toolchain/arm-cortex_a8-linux-gnueabihf:arm-cortex_a8-linux-gnueabihf.BUILD"),
#            url = URL_TOOLCHAIN + "arm-cortex_a8-linux-gnueabihf.tar.gz",
#            sha256 = "bf731dcdc1b8e925334a26d01d822f5e65efc580785f9fcbf90405b9397f16a8",
#        )

#    if "arm-cortex_a8-linux-gnueabihf-sysroot" not in native.existing_rules():
#        http_archive(
#            name = "arm-cortex_a8-linux-gnueabihf-sysroot",
#            build_file = Label("//bazel/toolchain/arm-cortex_a8-linux-gnueabihf:arm-cortex_a8-linux-gnueabihf-sysroot.BUILD"),
#            url = URL_SYSROOT + "beaglebone.tar.gz",
#            sha256 = "c716440776ec0e8823e226268ce490bf7e705c2c869e41b1bebcf26ff99fd19d",
#        )
