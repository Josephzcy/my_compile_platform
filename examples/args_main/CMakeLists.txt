cmake_minimum_required(VERSION 3.10)
project(args_main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(GLOG_DIR ../../third_party/linux94/glog_0.3.5-rc.5)
set(GFLAGS_DIR ../../third_party/linux94/gflags_2.2.2)
include_directories(
    config
    core
    src
    ${GLOG_DIR}/include
    ${GFLAGS_DIR}/include
)
link_directories(
    ${GLOG_DIR}/lib
    ${GFLAGS_DIR}/lib
    )
aux_source_directory(config CONFIG)
aux_source_directory(core CORE)
aux_source_directory(src SRC)


set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

add_library(config STATIC ${CONFIG})
add_library(core STATIC ${CORE})
add_library(src STATIC ${SRC})
add_executable(${PROJECT_NAME} args_main.cc)
target_link_libraries(${PROJECT_NAME} config core src glog gflags)