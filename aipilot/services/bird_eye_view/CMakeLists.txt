CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(bird_eye_view)
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
INCLUDE(${BUILD_SCRIPT_ROOT}/cmake/common.cmake) 
INCLUDE(${BUILD_SCRIPT_ROOT}/tda4.toolchain.cmake)

LINK_DIRECTORIES(
  vendor/libtda4/lib
  ${TDA4_SYS_ROOT}/lib
  ${TDA4_SYS_ROOT}/usr/lib
)

aux_source_directory(src SRC_LIST)
aux_source_directory(utils UTILS_LIST)
set(MAIN_FILE main_bev.cpp)
set(PRORO_LIST 
  ${AIPLORE_ROOT}/msg/precompiler/camera.pb.cc
  ${AIPLORE_ROOT}/msg/precompiler/vehicle_signal.pb.cc
  ${AIPLORE_ROOT}/msg/precompiler/apa_state.pb.cc
)

ADD_EXECUTABLE(${PROJECT_NAME}
  ${SRC_LIST}
  ${UTILS_LIST}
  ${MAIN_FILE}
  ${PRORO_LIST}
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE
  ${AIPLORE_ROOT}
	${TDA4_SYS_ROOT}/usr/include/processor_sdk/vision_apps/utils/mem/include
	${TDA4_SYS_ROOT}/usr/include/processor_sdk/tiovx/include
	${TDA4_SYS_ROOT}/usr/include/processor_sdk/tiovx/kernels/include
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  ${OPENCV_LIBS}
  DiagAgent
  pthread
  app_utils_minicv
  vx_kernels_img_proc
  dds
  dds_api
  flow
  nanomsg
  protobuf
  gflags
  glog
)

