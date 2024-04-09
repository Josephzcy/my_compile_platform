#!/bin/bash
#set -x
var=$1

RED_COLOR='\E[1;31m'   #红
GREEN_COLOR='\E[1;32m' #绿
YELOW_COLOR='\E[1;33m' #黄
BLUE_COLOR='\E[1;34m'  #蓝
RES='\E[0m'

checkout_branch() {
    find_item=`git branch | grep $branch`
    if [ ${#find_item} -ne 0 ]; then
        git checkout $branch
    else
        echo "Warning : has no branch $branch"
        return 1
    fi
    return 0
}

remove_self_deps() {
    module_name=$1
    conanfile=$2
    echo -e "${RED_COLOR}$conanfile${RES}"
    conan_pkg_name=`echo ${module_name} | awk -F'dfg_' '{print $2}'`
    if [ "${conan_pkg_name}" = "apa" ]; then
        return
    fi
    if [ ${#conan_pkg_name} -eq 0 ]; then
        conan_pkg_name=${module_name}
    fi
    echo -e "${BLUE_COLOR}$conan_pkg_name${RES}"
    sed -i "/${conan_pkg_name}\//d" $conanfile
}

#echo -e "${BLUE_COLOR}========================> BrachName = ${branch} <======================== ${RES}"
echo -e "${GREEN_COLOR}===================== Project: $var ==================================${RES}"

script_root=`pwd`/cmakebuild
cd ..
aiplore_root=`pwd`

project_path=${aiplore_root}/aipilot/modules/$var
project_path_services=${aiplore_root}/aipilot/services/$var
project_path_tools=${aiplore_root}/tools/$var
project_path_examples=${aiplore_root}/examples/$var
if [ ! -d "$project_path" ] && [ ! -d "$project_path_services" ] && [ ! -d "$project_path_examples" ] && [ ! -d "$project_path_tools" ]; then
    echo -e "${RED_COLOR} $var not exist ${RES}"
    exit -1
elif [ -d "$project_path" ]; then
    echo "find $var"
elif [ -d "$project_path_services" ]; then
    project_path="$project_path_services"
elif [ -d "$project_path_examples" ]; then
    project_path="$project_path_examples"
elif [ -d "$project_path_tools" ]; then
    project_path="$project_path_tools"
fi

echo -e "${YELOW_COLOR}=============project_path:${project_path} ==============${RES}"
pushd $project_path

# 切换到对应分支
# checkout_branch
# if [ $? -ne 0 ]; then
#     echo -e "${RED_COLOR} $branch not exit in $var ${RES}"
# fi

# create build directory
build_dir=$project_path/build/$platform
if [ ! -d "${build_dir}" ];then
    mkdir -p ${build_dir}/install
fi
pushd ${build_dir}
echo -e ${BLUE_COLOR}`pwd`${RES}
/bin/rm * -rf

# build procedure
if [ $platform = "linux" ]; then
#   cp ${script_root}/conanfile/linux.txt .
    cp ${script_root}/cmake/deps.linux.cmake deps.cmake   
#   remove_self_deps $var linux.txt
#   conan install linux.txt -s os=Linux -s arch=x86_64 -s compiler.version=4.9
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_INSTALL_PREFIX=${aiplore_root}/${branch}/${platform}/$var \
        ${config[*]} \
        -DCPU_ONLY=ON \
        -DBUILD_SCRIPT_ROOT=${script_root} \
        -DAIPLORE_ROOT=${aiplore_root}     \
        $project_path
elif [ $platform = "tda4" ]; then
#   cp ${script_root}/conanfile/tda4.txt .
#   remove_self_deps $var tda4.txt
    cp ${script_root}/cmake/deps.tda4.cmake deps.cmake       
#   conan install tda4.txt -s os=Linux -s arch=armv8 -s compiler.version=9.2
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_INSTALL_PREFIX=${aiplore_root}/${branch}/${platform}/$var \
        -DBUILD_SCRIPT_ROOT=${script_root} \
        -DAIPLORE_ROOT=${aiplore_root}     \
        -DCMAKE_SYSTEM_NAME=Linux \
        -DCMAKE_CXX_COMPILER=${tda4_build_toolchain_root}/bin/aarch64-none-linux-gnu-c++ \
        -DCMAKE_C_COMPILER=${tda4_build_toolchain_root}/bin/aarch64-none-linux-gnu-gcc \
        -DCMAKE_AR=${tda4_build_toolchain_root}/bin/aarch64-none-linux-gnu-gcc-ar \
        -DPLATFORM=${platform} \
        ${config[*]} \
        $project_path        
elif [ $platform = "J3" ]; then
    cp ${script_root}/cmake/deps.j3.cmake deps.cmake    
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_INSTALL_PREFIX=${build_dir}/install \
        -DBUILD_SCRIPT_ROOT=${script_root} \
        -DAIPLORE_ROOT=${aiplore_root}     \
        -DCMAKE_SYSTEM_NAME=Linux \
        -DCMAKE_CXX_COMPILER=${j3_build_toolchain_root}/bin/aarch64-linux-gnu-c++ \
        -DCMAKE_C_COMPILER=${j3_build_toolchain_root}/bin/aarch64-linux-gnu-gcc \
        -DCMAKE_AR=${j3_build_toolchain_root}/bin/aarch64-linux-gnu-gcc-ar \
        -DPLATFORM=${platform} \
        ${config[*]} \
        $project_path
elif [ $platform = "armv8-android" ]; then
    cp ${script_root}/conanfile/m4.txt .
    # 防止依赖conan包里对应版本的头文件
    remove_self_deps $var m4.txt
    conan install m4.txt -s os=Android -s arch=armv8 -s os.api_level=android-21
    cmake clean
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_TOOLCHAIN_FILE=${script_root}/android.toolchain.cmake \
        -DANDROID_NATIVE_API_LEVEL=android-21 \
        -DANDROID_ABI=arm64-v8a \
        -DBUILD_SCRIPT_ROOT=${script_root} \
        ${config[*]} \
        ../..
elif [ $platform = "armv8-linux" ]; then
    cp ${script_root}/conanfile/x2.txt .
    remove_self_deps $var x2.txt
    conan install x2.txt -s os=Linux -s arch=armv8 -s compiler.version=7.2
    cmake clean
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_SYSTEM_NAME=Linux \
        -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ \
        -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
        -DZU3=1 \
        -DBUILD_SCRIPT_ROOT=${script_root} \
        ${config[*]} \
        ../..
elif [ $platform = "armv7-linux" ]; then
    cp ${script_root}/conanfile/x1.txt .
    remove_self_deps $var x1.txt
    conan install x1.txt -s os=Linux -s arch=arm_zynq
    cmake clean
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_SYSTEM_NAME=Linux \
        -DCMAKE_CXX_COMPILER=/usr/bin/arm-linux-gnueabihf-g++-4.9 \
        -DCMAKE_C_COMPILER=/usr/bin/arm-linux-gnueabihf-gcc-4.9 \
        -DCMAKE_AR=/usr/bin/arm-linux-gnueabihf-gcc-ar-4.9 \
        -DBUILD_SCRIPT_ROOT=${script_root} \
        ${config[*]} \
        ../..
elif [ $platform = "cv22" ]; then
    cp ${script_root}/conanfile/cv22.txt .
    remove_self_deps $var cv22.txt
    source ${cv22_sw_sdk_dir}/setupenv.sh
    conan install cv22.txt -s os=Linux -s arch=armv8 -s compiler.version=8.2
    cmake clean
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DBUILD_SCRIPT_ROOT=${script_root} \
        ${config[*]} \
        ../..
elif [ $platform = "armv7-android" ]; then
    cp ${script_root}/conanfile/arm32.txt .
    remove_self_deps $var arm32.txt
    conan install arm32.txt -s os=Android -s os.api_level=android-21 -s arch=armv7
    cmake clean
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_TOOLCHAIN_FILE=${script_root}/android.toolchain.cmake \
        -DANDROID_NATIVE_API_LEVEL=android-21 \
        -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 \
        -DANDROID_ABI="armeabi-v7a with NEON" \
        -DBUILD_SCRIPT_ROOT=${script_root} \
        ${config[*]} \
        ../..
fi

if [ 0 -ne $? ]; then
    echo -e "${RED_COLOR}Error : cmake failed ......${RES}"
    exit 1
fi
make clean
make -j $(nproc)
#make install

if [ 0 -ne $? ]; then
    echo -e "${RED_COLOR}Error : build failed ......${RES}"
    exit 1
fi

echo -e ${RED_COLOR}`pwd`${RES}

if [ "${branch}" != "cicd"  ];then 
  if [ ! -e runtime ];then
      mkdir -p runtime
      echo "mkdir runtime"
  fi

#  sed '1,/LINK_DIRECTORIES/d' deps.cmake  |grep -vE "\)|^$" | while read line
  sed '1,/LINK_DIRECTORIES/d' deps.cmake  |grep "AIPLORE_ROOT" | while read line
  do
  #  echo $line
     line=`echo $line|awk -F"}/" '{$1="";print}' | awk '{print $1}'`
  #  echo ${aiplore_root}/${line}
  #  echo [$line]
     cp -rf ${aiplore_root}/${line}/* runtime/
  done

  cd runtime
  rm -rf *.a cmake *.cmake *.pc
fi

popd
popd

