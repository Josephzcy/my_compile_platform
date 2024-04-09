cmake_minimum_required(VERSION 3.10)
PROJECT(dfg_avm_mapping VERSION "1.0.9")

# get git hash
macro(get_git_commit _git_commit)   # 宏的开始
    find_package(Git QUIET)     # 查找Git，QUIET静默方式不报错
    if(GIT_FOUND)
        execute_process(          # 执行一个子进程
                COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h # 命令
                OUTPUT_VARIABLE ${_git_commit}        # 输出字符串存入变量
                OUTPUT_STRIP_TRAILING_WHITESPACE    # 删除字符串尾的换行符
                ERROR_QUIET                         # 对执行错误静默
                WORKING_DIRECTORY                   # 执行路径
                ${CMAKE_CURRENT_SOURCE_DIR}
                )
    endif()
endmacro()


string(TIMESTAMP BUILD_TIME1 %Y%m%d_%H%M%S)
string(TIMESTAMP BUILD_TIME2 %Y%m%d%H%M)
set(build_time1 ${BUILD_TIME1})
set(build_time2 ${BUILD_TIME2})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/version.h)

SET(BUILD_SCRIPT_ROOT ${AIPLORE_ROOT}/scripts/build_scripts_apa)
INCLUDE(${BUILD_SCRIPT_ROOT}/cmake/common.cmake)
IF (WITH_LINUX_PC)
    INCLUDE(${BUILD_SCRIPT_ROOT}/cmake/deps.linux.cmake)
    SET(OPENCV_LIB -lopencv_imgproc -lopencv_core -lopencv_calib3d -lopencv_imgcodecs -lopencv_highgui)
ELSE ()
    INCLUDE(${BUILD_SCRIPT_ROOT}/tda4.toolchain.cmake)
    INCLUDE(${BUILD_SCRIPT_ROOT}/cmake/deps.tda4.cmake)
    SET(OPENCV_LIB -lopencv_imgproc -lopencv_core -lopencv_calib3d -lopencv_imgcodecs)
ENDIF ()
SET(PB_PATH  ${BUILD_SCRIPT_ROOT}/../../msg/precompiler)
INCLUDE_DIRECTORIES(
        ${PB_PATH}
        ${AIPLORE_ROOT}
)
link_libraries(-lprotobuf -lglog -lgflags ${OPENCV_LIB})

aux_source_directory(src SRC_LIST)
aux_source_directory(camera CAMERA_SOURCE)
list(APPEND SRC_LIST avm_mapping.cpp ${CAMERA_SOURCE} ${PB_PATH}/camera.pb.cc ${PB_PATH}/avm_calib_param.pb.cc )
IF(NOT FOR_AIPLORER)
    IF (BUILD_SHARED_LIBS)
        add_library(${PROJECT_NAME} SHARED ${SRC_LIST} )
    ELSE ()
        add_library(${PROJECT_NAME} STATIC ${SRC_LIST} )
    ENDIF ()
ELSE()
    add_library(${PROJECT_NAME} STATIC ${SRC_LIST} )
ENDIF()
