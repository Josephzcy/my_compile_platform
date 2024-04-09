CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
# The version number.
SET(Tutorial_VERSION_MAJOR 0)
SET(Tutorial_VERSION_MINOR 1)

SET(CMAKE_CXX_STANDARD 11)
IF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    SET(BUILD_TYPE debug)
ELSE()
    SET(BUILD_TYPE release)
ENDIF()

# lib path
IF(ANDROID_ABI)
    SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -O3 -std=c++11 -fPIC -flax-vector-conversions")
    SET(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -g -std=c++11 -fPIC -flax-vector-conversions")
ELSEIF(WITH_FPGA_ACCELERATE)
    SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -mfp16-format=ieee -mfpu=neon-fp16 -O3 -DUSE_GLOG=1 -std=c++11 -fPIC -flax-vector-conversions")
    SET(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -mfp16-format=ieee -mfpu=neon-fp16 -g -std=c++11 -fPIC -flax-vector-conversions")
ELSEIF(UNIX)
    SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -O2 -DUSE_GLOG=1 -fPIC -flax-vector-conversions -DNDEBUG")
    SET(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable -g -O0 -fPIC -flax-vector-conversions")
ENDIF ()

SET(PROJECT_ROOT_PATH "${CMAKE_SOURCE_DIR}")
MESSAGE(STATUS "${CMAKE_SOURCE_DIR}")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(TESTS_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin/tests")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


# config
OPTION(WITH_TEST "whether compile test program" OFF)
OPTION(WITH_FPGA_ACCELERATE "switch to fpga, camera triggered" OFF)
OPTION(WITH_NEON "whether use neon" OFF)
OPTION(WITH_ZU3 "whether on zu3 platform" OFF)
OPTION(WITH_BFS_ACCELERATE "whether open bfs accelerate" OFF)
OPTION(BUILD_SHARED_LIBS "whehter build shared libs" ON)
OPTION(WITH_GSENSOR_LOG "whether use gsensor log" OFF)
OPTION(WITH_MINIDNN "switch to M4/M3, camera triggered" OFF)
OPTION(WITH_CV22_CHIP "whether on cv22 platform" OFF)
OPTION(WITH_CV22 "product of cv22" OFF)
OPTION(WITH_AVM "product of avm" OFF)
OPTION(WITH_M4 "product of m4" OFF)
OPTION(WITH_AEB "product of aeb" OFF)
OPTION(WITH_X1D "product of x1d" OFF)
OPTION(WITH_LINUX_PC "switch to compile alg for linux platform" OFF)
OPTION(WITH_GFLAGS_PARA "switch to for para from gflags" OFF)
OPTION(WITH_GPU "switch to open gpu" OFF)
OPTION(WITH_TDA4 "switch to TDA4 " OFF)
OPTION(WITH_J3 "switch to J3 " OFF)


IF(NOT FOR_AIPLORER)
    INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
ELSE()
    INCLUDE(${CMAKE_BINARY_DIR}/deps.cmake)
ENDIF()

IF(NOT WITH_CV22_CHIP)
    MESSAGE(STATUS "NOT WITH_CV22_CHIP")
ELSE()
    ADD_DEFINITIONS(-DCHIP_CV22)
    ADD_DEFINITIONS(-DCV22)
ENDIF()

IF (ANDROID_ABI)
    MESSAGE(STATUS "ANDROID_ABI ON")
    ADD_DEFINITIONS(-DANDROID)
ENDIF()

IF(WITH_TDA4)
	MESSAGE(STATUS "WITH_TDA4 ON")
	ADD_DEFINITIONS(-DWITH_TDA4)
ENDIF()

IF(WITH_J3)
	MESSAGE(STATUS "WITH_J3 ON")
	ADD_DEFINITIONS(-DWITH_J3)
ENDIF()

IF(WITH_FPGA_ACCELERATE)
    MESSAGE(STATUS "WITH_FPGA_ACCELERATE ON")
    ADD_DEFINITIONS(-DFPGA_ACCELERATE)
    SET(FPGA_ACCELERATE 1)
    IF(WITH_ZU3)
    ELSE()
        ADD_COMPILE_OPTIONS(-mfpu=neon)
    ENDIF()
ENDIF()

IF(WITH_NEON)
    MESSAGE(STATUS "WITH_NEON ON")
    ADD_DEFINITIONS(-DUSE_NEON)
ENDIF()

IF(WITH_BFS_ACCELERATE)
    ADD_DEFINITIONS(-DBFS_ACCELERATE)
    MESSAGE(STATUS "WITH_BFS_ACCELERATE ON")
ENDIF()

IF(WITH_GSENSOR_LOG)
    ADD_DEFINITIONS(-DUSE_GSENSOR_LOG)
    MESSAGE(STATUS "WITH_GSENSOR_LOG ON")
ENDIF()

IF(WITH_MINIDNN)
    MESSAGE(STATUS "WITH_MINIDNN ON")
    ADD_DEFINITIONS(-DMINIDNN)
ENDIF()

IF(WITH_CV22)
    MESSAGE(STATUS "WITH_CV22 ON")
    ADD_DEFINITIONS(-DCV22)
ENDIF()

IF(WITH_AVM)
    MESSAGE(STATUS "WITH_AVM ON")
    ADD_DEFINITIONS(-DAVM)
ENDIF()

IF(WITH_M4)
    MESSAGE(STATUS "WITH_M4 ON")
    ADD_DEFINITIONS(-DM4)
ENDIF()

IF(WITH_AEB)
    MESSAGE(STATUS "WITH_AEB ON")
    ADD_DEFINITIONS(-DAEB)
ENDIF()

IF(WITH_X1D)
    MESSAGE(STATUS "WITH_X1D ON")
    ADD_DEFINITIONS(-DX1D)
ENDIF()

IF(WITH_LINUX_PC)
    MESSAGE(STATUS "WITH_LINUX_PC ON")
    ADD_DEFINITIONS(-DWITH_LINUX_PC)
ENDIF()

IF(WITH_CV22_CHIP)
    MESSAGE(STATUS "WITH_CV22_CHIP ON")
    INCLUDE_DIRECTORIES(
        ${CV22_INCLUDE_DIR}
        # ${INSTALL_INCLUDE_DIR}
        ${CV22_INCLUDE_DIR}/eigen3
        )
    LINK_DIRECTORIES(
        ${CV22_LIB_DIR}
        # ${INSTALL_LIB_DIR}
        )
ELSE()
    INCLUDE_DIRECTORIES(
        # ${INSTALL_INCLUDE_DIR}
        )
    LINK_DIRECTORIES(
        # ${INSTALL_LIB_DIR}
        )
ENDIF()

IF(WITH_GFLAGS_PARA)
    MESSAGE(STATUS "WITH_GFLAGS_PARA ON")
    ADD_DEFINITIONS(-DPARA_FROM_GFLAGS)
ENDIF()

SET(ANDROID_OPENCV_3RDPARTY_LIBS libIlmImf.a  liblibjasper.a  liblibjpeg.a  liblibpng.a  liblibtiff.a  liblibwebp.a)

IF(ANDROID_ABI)
    SET(OPENCV_LIBS opencv_video opencv_ml opencv_videoio opencv_imgproc opencv_core opencv_imgcodecs opencv_hal opencv_shape opencv_objdetect opencv_calib3d)
    SET(GLOG_LIBS miniglog)
ELSEIF(WITH_CV22_CHIP)
    SET(OPENCV_LIBS opencv_video opencv_ml opencv_videoio opencv_imgproc opencv_core opencv_imgcodecs opencv_highgui opencv_objdetect opencv_calib3d)
    SET(GLOG_LIBS glog)
ELSEIF(WITH_TDA4)
    SET(OPENCV_LIBS opencv_video opencv_ml opencv_videoio opencv_imgproc opencv_core opencv_imgcodecs opencv_shape opencv_stitching opencv_superres opencv_videostab opencv_photo opencv_highgui opencv_objdetect opencv_calib3d opencv_features2d opencv_features2d opencv_flann)
ELSEIF(WITH_J3)
    SET(OPENCV_LIBS opencv_video opencv_ml opencv_videoio opencv_imgproc opencv_core opencv_imgcodecs opencv_shape opencv_stitching opencv_superres opencv_videostab opencv_photo opencv_highgui opencv_objdetect opencv_calib3d opencv_features2d opencv_features2d opencv_flann)
    SET(GLOG_LIBS glog)
ELSEIF(WITH_FPGA_ACCELERATE)
    SET(OPENCV_LIBS opencv_video opencv_ml opencv_videoio opencv_imgproc opencv_core opencv_imgcodecs opencv_shape opencv_objdetect opencv_calib3d)
    SET(GLOG_LIBS miniglog)
ELSE()
    SET(OPENCV_LIBS opencv_video opencv_ml opencv_videoio opencv_imgproc opencv_core opencv_imgcodecs opencv_shape opencv_stitching opencv_superres opencv_videostab opencv_photo opencv_highgui opencv_objdetect opencv_calib3d opencv_features2d opencv_features2d opencv_flann)
    SET(GLOG_LIBS glog)
ENDIF()

IF(WITH_CV22_CHIP OR ANDROID_ABI)
    SET(COMMON_LIBS coordinate_trans base string concurrency time timer config)
ELSE()
    SET(COMMON_LIBS common)
ENDIF()

IF(WITH_GPU)
    MESSAGE(STATUS "GPU_OPEN")
ELSE()
    MESSAGE(STATUS "CPU_OPEN")
    ADD_DEFINITIONS(-DCPU_ONLY)
    SET(CAFFE_LIBS caffe)
ENDIF()
