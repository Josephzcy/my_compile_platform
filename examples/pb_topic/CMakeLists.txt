cmake_minimum_required(VERSION 3.5)
PROJECT(pb_topic)

INCLUDE(${BUILD_SCRIPT_ROOT}/cmake/common.cmake)

message(STATUS,"BUILD_SCRIPT_ROOT:${BUILD_SCRIPT_ROOT}")

SET(CUR_PROTO_GEN ${BUILD_SCRIPT_ROOT}/../../msg/precompiler)
#SET(PB_DIR ${BUILD_SCRIPT_ROOT}/../../msg/pb)
#SET(GLOG_DIR ${BUILD_SCRIPT_ROOT}/../../third_party/linux94/glog_0.3.5-rc.5)
#SET(NANOMSG_DIR ../../third_party/linux94/nanomsg_1.1.5-rc.0)
#SET(DDS_DIR  ${BUILD_SCRIPT_ROOT}/../../common/linux94/dds_1.14.0)
INCLUDE_DIRECTORIES(
    ${CUR_PROTO_GEN}
#    ${PB_DIR}    
#    ${GLOG_DIR}/include
#    ${DDS_DIR}/include
)

aux_source_directory(src SRC_LIST)
LIST(APPEND SRC_LIST ${CUR_PROTO_GEN}/camera.pb.cc)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/$(ADD_DIRS)/bin)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} 
    dds
    dds_api
    pthread
    protobuf
    nanomsg
)
