def register_all_toolchains():
    native.register_toolchains(
        "//bazel/toolchain/aarch64-none-linux-gnu:aarch64_linux_toolchain",
        "//bazel/toolchain/aarch64-j3-linux-gnu:aarch64_linux_toolchain",        
        "//bazel/toolchain/arm-cortex_a8-linux-gnueabihf:armv7_linux_toolchain",
    )

