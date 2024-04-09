#!/bin/bash
platform=$1
build_mode=$2
project=$3
para_mode=$4

force_compile=true
compile_dir=""


# 必备功能
# sys
os_env=linux
sys_arch=x64

armv8_android=OFF
armv8_linux=OFF
armv7_android=OFF
armv7_linux=OFF
zu3_set=OFF
cv22_chip=OFF
cv22_set=OFF
bfs_accelerate=OFF
shared_libs=ON
cpu_only=ON
compile_test=OFF
use_neon=OFF
use_gsensor_log=OFF
use_minidnn=OFF
avm_set=OFF
m4_set=OFF
aeb_set=OFF
x1d_set=OFF
linux_pc=OFF
para_from_gflags=OFF
tda4_on=OFF
J3_on=OFF
pb2_on=OFF

# common 仓库切换成对应的版本
if [ $platform = "armv8-android" ]; then
    echo "=====>Armv8-android(M3/M4) Platform<====="
    compile_dir=armv8-android
    os_env=armv8-android
    sys_arch=arm64
    armv8_android=ON

    use_neon=ON
    debug_show_mode=OFF
    debug_save_mode=OFF
    display_result=OFF
    use_minidnn=ON
    shared_libs=OFF
elif [ $platform = "linux" ]; then
    echo "=====>Linux Platform<====="
    compile_dir=linux
    os_env=linux
    sys_arch=x64
    display_result=ON
    debug_save_mode=OFF
    debug_show_mode=OFF
    build_tool=ON
    use_gsensor_log=ON
    linux_pc=ON
elif [ $platform = "armv7-linux" ]; then
    echo "=====>Armv7-linux(X1) Platform <====="
    compile_dir=armv7-linux
    os_env=armv7-linux
    sys_arch=arm
    armv7_linux=ON

    fpga_accelerate=ON
    use_neon=ON
    debug_save_mode=OFF
    debug_show_mode=OFF
elif [ $platform = "armv8-linux" ]; then
    echo "=====>armv8-linux(ZU3) Platform <====="
    compile_dir=armv8-linux
    os_env=armv8-linux
    sys_arch=arm64
    armv8_linux=ON

    fpga_accelerate=ON # to be verified
    os_env=armv8-linux
    use_neon=ON
    zu3_set=ON
    debug_save_mode=OFF
    debug_show_mode=OFF
elif [ $platform = "armv7-android" ]; then
    echo "======> Armv7-android(F1) Platform"
    compile_dir=armv7-android
    os_env=armv7-android
    sys_arch=arm
    armv7_android=ON

    use_ncnn=ON
    use_neon=ON
    debug_show_mode=OFF
    debug_save_mode=OFF
    display_result=OFF
elif [ $platform = "cv22" ]; then
    echo "======> CV22 Platform"
    compile_dir=cv22
    os_env=armv8
    sys_arch=armv8

    use_neon=ON
    debug_show_mode=OFF
    debug_save_mode=OFF
    display_result=OFF
elif [ $platform = "tda4" ]; then
    echo "======> tda4 Platform"
    compile_dir=tda4
    os_env=linux
    sys_arch=armv8
    armv8=ON

    use_neon=ON
    tda4_on=ON
    tda4_sys_root=/opt/toolchains/tda4/sysroots
    tda4_build_toolchain_root=/opt/toolchains/tda4/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu
    tda4_sys_include_dir=${tda4_build_toolchain_root}/lib/gcc/aarch64-none-linux-gnu/9.2.1/include/
elif [ $platform = "J3" ]; then
    echo "======> J3 Platform"
    os_env=linux
    sys_arch=armv8
    armv8=ON
    use_neon=ON
    J3_on=ON
    pb2_on=ON
    j3_sys_root=/opt/toolchains/J3/sysroots/usr
    j3_build_toolchain_root=/opt/toolchains/J3/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu
    j3_sys_include_dir=${j3_build_toolchain_root}/lib/gcc/aarch64-linux-gnu/6.5.0/include/
else
    echo "Error : Unsupport Platform :"${platform}
    exit 0
fi

cv22_sys_include_dir=""
if [ ${platform} = "cv22" ]; then
    cv22_chip=ON
    cv22_set=ON
    cv22_sw_sdk_dir=${HOME}/data3/amba/a1ba-sw-sdk/
    cv22_sys_include_dir=${cv22_sw_sdk_dir}/usr/aarch64-buildroot-linux-gnu/sysroot/usr/include/linux/
    cv22_include_dir=${cv22_sw_sdk_dir}/usr/aarch64-buildroot-linux-gnu/sysroot/usr/include/
    cv22_lib_dir=${cv22_sw_sdk_dir}/usr/aarch64-buildroot-linux-gnu/sysroot/usr/lib/
elif [ ${platform} = "armv7-linux" ]; then
    bfs_accelerate=ON
elif [ ${platform} = "armv8-android" ]; then
    arm_set=ON
elif [ ${platform} = "linux" ]; then
    echo "linux platform ..."
    compile_test=ON
    linux_pc=ON
fi

#install_include_dir=${HOME}/data3/apa_repo/dfg_apa/include/
#install_lib_dir=${HOME}/data3/apa_repo/dfg_apa/lib/${platform}

if [ ${para_mode} = "gflags" ]; then
    para_from_gflags=ON
fi

config=(
#"-DCMAKE_INSTALL_PREFIX=${install_prefix}"
"-DOS_ENV=${os_env}"
"-DSYS_ARCH=${sys_arch}"
"-DCMAKE_BUILD_TYPE=${build_mode}"
"-DWITH_FPGA_ACCELERATE=${fpga_accelerate}"
"-DWITH_NEON=${use_neon}"
"-DWITH_CPU_ONLY=${cpu_only}"
"-DWITH_TEST=${compile_test}"
"-DWITH_ZU3=${zu3_set}"
#"-DWITH_CV22_CHIP=${cv22_chip}"
"-DANDROID_ARMV7=${armv7_android}"
"-DANDROID_ARMV8=${armv8_android}"
#"-DCV22_INCLUDE_DIR=${cv22_include_dir}"
#"-DCV22_LIB_DIR=${cv22_lib_dir}"
"-DINSTALL_INCLUDE_DIR=${install_include_dir}"
"-DINSTALL_LIB_DIR=${install_lib_dir}"
"-DBUILD_SHARED_LIBS=${shared_libs}"
#"-DCV22_SYS_INCLUDE_DIR=${cv22_sys_include_dir}"
"-DWITH_BFS_ACCELERATE=${bfs_accelerate}"
"-DWITH_GSENSOR_LOG=${use_gsensor_log}"
"-DWITH_MINIDNN=${use_minidnn}"
#"-DWITH_CV22=${cv22_set}"
"-DWITH_AVM=${avm_set}"
"-DWITH_M4=${m4_set}"
"-DWITH_AEB=${aeb_set}"
"-DWITH_X1D=${x1d_set}"
"-DWITH_LINUX_PC=${linux_pc}"
"-DWITH_GFLAGS_PARA=${para_from_gflags}"
"-DWITH_GPU=${gpu_on}"
"-DWITH_TDA4=${tda4_on}"
"-DWITH_J3=${J3_on}"
"-DWITH_PB2=${pb2_on}"
"-DTDA4_SYS_ROOT=${tda4_sys_root}"
"-DJ3_SYS_ROOT=${j3_sys_root}"
"-DFOR_AIPLORER=ON"
 )
