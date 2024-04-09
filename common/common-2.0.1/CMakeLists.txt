cmake_minimum_required(VERSION 3.5)
project(common_repo)
set(CMAKE_CXX_STANDARD 11)

# add by xuzhixiang
add_definitions("-fPIC")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

##############
## Platform ##
##############
IF (ANDROID_ABI)
    SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -O3 -std=c++11")
    SET(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -g  -std=c++11")
    
    ADD_DEFINITIONS(-DANDROID)
    ADD_DEFINITIONS(-DUSE_CAMERA) # 新摄像头
    ADD_DEFINITIONS(-DJSON_ANDROID_WORKAROUNDS)

    if (${ANDROID_ABI} STREQUAL "arm64-v8a")
        SET(PLATFORM_ARMV8 ON)
    elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a with NEON")
        SET(PLATFORM_ARMV7 ON)
    endif ()

ELSEIF (ARM_LINUX_ZYNQ)
    #SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -mfp16-format=ieee -mfpu=neon-fp16 -O3 -std=gnu++14")
    #SET(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -mfp16-format=ieee -mfpu=neon-fp16 -g -std=gnu++14")
    #SET(CMAKE_C_FLAGS_RELEASE "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -mfp16-format=ieee -mfpu=neon-fp16 -O3 -DUSE_GLOG=1")
    #SET(CMAKE_C_FLAGS_DEBUG "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -mfp16-format=ieee -mfpu=neon-fp16 -g")
    #SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")
    SET(CMAKE_CXX_FLAGS_RELEASE "-Wno-unused-result -Wno-strict-aliasing -Wno-narrowing -Wno-unused-function -DCR_DEBUG_FLAG -DNDEBUG -O3 -Wall -std=c++11")
    SET(CMAKE_CXX_FLAGS_DEBUG "-Wno-unused-result -Wno-strict-aliasing -Wno-narrowing -Wno-unused-function -Wno-unused-variable -Wall -DCR_DEBUG_FLAG -g -std=c++11")

    ADD_DEFINITIONS(-DFPGA_ACCELERATE)
    ADD_DEFINITIONS(-DARM_LINUX_ZYNQ)
    ADD_DEFINITIONS(-DX1_SPEAKER)

    SET(PLATFORM_ARM_ZYNQ ON)

ELSEIF (UNIX)
    SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -O3 -std=c++11")
    SET(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -g -O0 -std=c++11")
     
    ADD_DEFINITIONS(-DLINUX_PC)
    ADD_DEFINITIONS(-DCPU_ONLY)

    SET(PLATFORM_LINUX ON)
ENDIF ()

# OpenCV
SET(Android_OpenCV_LIBS
        libopencv_imgproc.a
        libopencv_calib3d.a
        libopencv_videoio.a
        libopencv_core.a
        libopencv_features2d.a
        libopencv_flann.a
        libopencv_highgui.a
        libopencv_imgcodecs.a
        libopencv_ml.a
        libopencv_objdetect.a
        libopencv_photo.a
        libopencv_shape.a
        libopencv_stitching.a
        libopencv_superres.a
        libopencv_video.a
        libopencv_videostab.a
        libpng libtiff libjpeg-turbo libjasper libwebp tegra_hal ittnotify IlmImf log zlib)
if (PLATFORM_ARMV8)
    SET(OpenCV_LIBS ${Android_OpenCV_LIBS})
elseif (PLATFORM_ARMV7_ANDROID)
    SET(OpenCV_LIBS ${Android_OpenCV_LIBS})
elseif (PLATFORM_ARMV7)
    LIST(APPEND Android_OpenCV_LIBS tbb)
    SET(OpenCV_LIBS ${Android_OpenCV_LIBS})
elseif (PLATFORM_ARM_ZYNQ)
    SET(OpenCV_LIBS
        opencv_calib3d
        opencv_imgcodecs
        opencv_highgui
        opencv_videoio
        opencv_imgcodecs
        opencv_imgproc
        opencv_core
        libpng libtiff libjpeg-turbo libjasper libwebp IlmImf zlib dl)
elseif (PLATFORM_LINUX)
    SET(OpenCV_LIBS
        opencv_shape
        opencv_stitching
        opencv_objdetect
        opencv_superres
        opencv_videostab
        opencv_calib3d
        opencv_features2d
        opencv_highgui
        opencv_videoio
        opencv_imgcodecs
        opencv_video
        opencv_photo
        opencv_ml
        opencv_imgproc
        opencv_flann
        opencv_viz
        # ippicv
        opencv_core)
elseif (ARMV8_LINUX)
    SET(OpenCV_LIBS
            opencv_shape
            opencv_stitching
            opencv_objdetect
            opencv_superres
            opencv_videostab
            opencv_calib3d
            opencv_features2d
            opencv_highgui
            opencv_videoio
            opencv_imgcodecs
            opencv_video
            opencv_photo
            opencv_ml
            opencv_imgproc
            opencv_flann
            opencv_core
            dl
            m
            pthread
            rt)
endif ()

# conan
#include(${CMAKE_BINARY_DIR}/../../conanbuildinfo.cmake)
#conan_basic_setup()
include(${BUILD_SCRIPT_ROOT}/cmake/common.cmake)
if(USE_GLOG)
    ADD_DEFINITIONS("-DUSE_GLOG")
    SET(Glog_LIBS "glog")
endif()

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(..)

if(BUILD_SHARED_LIBS)
    SET(LIBS_TYPE "SHARED")
else()
    SET(LIBS_TYPE "STATIC")
endif()

# BUILD_APART_LIBS OR all_in_one: libcommon
if(BUILD_APART_LIBS)
    # BUILD_APART_LIBS
    ADD_SUBDIRECTORY(base)
    ADD_SUBDIRECTORY(time)
    ADD_SUBDIRECTORY(timer)
    ADD_SUBDIRECTORY(system)
    ADD_SUBDIRECTORY(concurrency)
    ADD_SUBDIRECTORY(memory)
    ADD_SUBDIRECTORY(mjpeg)
    ADD_SUBDIRECTORY(mp4)
    ADD_SUBDIRECTORY(string)
    ADD_SUBDIRECTORY(serialize)
    ADD_SUBDIRECTORY(file_operate)
    ADD_SUBDIRECTORY(fs)
    ADD_SUBDIRECTORY(ui)
    ADD_SUBDIRECTORY(config)
    ADD_SUBDIRECTORY(math)
    ADD_SUBDIRECTORY(coordinate_trans)
else()

    # all_in_one: libcommon
    aux_source_directory(base SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(time SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(timer SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(system SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(concurrency SUB_SRCS)
    LIST(REMOVE_ITEM SUB_SRCS "concurrency/module_parallel_test.cpp")
    LIST(REMOVE_ITEM SUB_SRCS "concurrency/thread_pool_test.cpp")
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(memory SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(mjpeg SUB_SRCS)
    LIST(REMOVE_ITEM SUB_SRCS "mjpeg/main.cpp")
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(mp4 SUB_SRCS)
    LIST(REMOVE_ITEM SUB_SRCS "mp4/main.cpp")
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(string SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(serialize SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(file_operate SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(fs SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(ui SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(config SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(math SUB_SRCS)
    LIST(REMOVE_ITEM SUB_SRCS "math/polyfit_test.cpp")
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})

    aux_source_directory(coordinate_trans SUB_SRCS)
    LIST(APPEND COMMON_SRCS ${SUB_SRCS})


    ADD_LIBRARY(common ${LIBS_TYPE} ${COMMON_SRCS})

    if (PLATFORM_ARMV8)
        set(OTHER_LIBS log opencv_core)
    endif()

    target_link_libraries(common ${Glog_LIBS} ${OpenCV_LIBS} ${OTHER_LIBS})
endif()

