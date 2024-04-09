cmake_minimum_required(VERSION 3.6)
PROJECT(avm_calib VERSION "1.0.4")

INCLUDE(${AIPLORE_ROOT}/scripts/build_scripts_apa/cmake/common.cmake)

macro(get_git_commit _git_commit)
    find_package(Git QUIET)
    if (GIT_FOUND)
        execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%H
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE GIT_REPO_HASH
                OUTPUT_STRIP_TRAILING_WHITESPACE
                )
    elseif ()
        message(STATUS "failed to find git")
    endif ()
endmacro()

IF (WITH_LINUX_PC)
    add_definitions(-DDEBUG)
    SET(OPENCV_LIB -lopencv_imgproc -lopencv_core -lopencv_calib3d -lopencv_imgcodecs -lopencv_highgui)
    list(APPEND OPENCV_LIB -lopencv_videoio)
ELSE ()
    SET(OPENCV_LIB -lopencv_flann -lopencv_features2d -lopencv_imgproc -lopencv_core -lopencv_calib3d -lopencv_imgcodecs)
ENDIF ()
INCLUDE_DIRECTORIES(
        ${AIPLORE_ROOT}
)
message(STATUS "BUILD_SCRIPT_ROOT:${BUILD_SCRIPT_ROOT}")
message(STATUS "AIPLORE_ROOT:${AIPLORE_ROOT}")

SET(BUILD_SHARED_LIBS OFF)
SET(PB_PATH ${AIPLORE_ROOT}/msg/precompiler)
SET(PB_FILE ${PB_PATH}/camera.pb.cc ${PB_PATH}/avm_calib_param.pb.cc ${PB_PATH}/avm_calib_ctrl.pb.cc)
ADD_SUBDIRECTORY(../dfg_avm_mapping lib)
ADD_SUBDIRECTORY(src)

IF (WITH_TDA4)
    ADD_SUBDIRECTORY(avm_calib_service)
    string(TIMESTAMP BUILD_TIME1 %Y%m%d_%H%M%S)
    string(TIMESTAMP BUILD_TIME2 %Y%m%d%H%M)
    set(build_time1 ${BUILD_TIME1})
    set(build_time2 ${BUILD_TIME2})

    # configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/../include/version.h)

    SET(SYSROOT /opt/toolchains/tda4/sysroots/)
    SET(SDK_PATH ${SYSROOT}/usr/include/processor_sdk/)
    message(STATUS "sdk_path:${SDK_PATH}")
    INCLUDE_DIRECTORIES(${SDK_PATH}/tidl_j7_02_00_00_07/ti_dl/inc)
    INCLUDE_DIRECTORIES(${SDK_PATH}/ivision)
    INCLUDE_DIRECTORIES(${SDK_PATH}/tiovx/include)
    INCLUDE_DIRECTORIES(${SDK_PATH}/tiovx/kernels_j7/include)
    INCLUDE_DIRECTORIES(${SDK_PATH}/tiovx/kernels/include)
    INCLUDE_DIRECTORIES(${SDK_PATH}/vision_apps)
    INCLUDE_DIRECTORIES(${SDK_PATH}/vision_apps/apps/basic_demos/app_tirtos/common)
    INCLUDE_DIRECTORIES(${SDK_PATH}/vision_apps/apps/basic_demos/app_tirtos/tirtos_linux)

    INCLUDE_DIRECTORIES(${SYSROOT}/usr/include)
    LINK_DIRECTORIES(${SYSROOT}/usr/lib)
    LINK_DIRECTORIES(${SYSROOT}/lib)

    add_executable(avm_calib_service avm_calib_task.cpp ${PB_FILE} ${CALIB_SOURCES})
    target_link_libraries(avm_calib_service
            dds_api
            dds
            tbb
            z
            rt
            dl
            pthread
            DiagAgent
            tivision_apps
            avm_calib
            avm_service
            dfg_avm_mapping
            )
ENDIF ()
