
# Dependencies:
#   cd vendor && git clone https://github.com/msgpack/msgpack-c

# IF(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
#   MESSAGE(STATUS "Platform: Darwin")

#   INCLUDE_DIRECTORIES(
#     "../src"
#   )
# ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES Linux)
#   MESSAGE(STATUS "Platform: Linux")

#   INCLUDE_DIRECTORIES(
#     "../src"
#   )
# ELSEIF(ANDROID_ABI)
#   MESSAGE(STATUS "Platform: ANDROID_ABI")

#   SET(CMAKE_SYSROOT "$ENV{ANDROID_NDK}/platforms/android-21/arch-arm64")

#   INCLUDE_DIRECTORIES(
#     "../src"
#   )
# ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(OpenCV_DIR "/Users/zhangqi/opt/opencv/3.4.2/share/OpenCV")
ENDIF()

FIND_PACKAGE(OpenCV REQUIRED)

#=======================================
ADD_EXECUTABLE(publish_video_server server.cpp)

TARGET_LINK_LIBRARIES(publish_video_server
  flow
  opencv_core
  opencv_videoio
)

#=======================================
ADD_EXECUTABLE(publish_video_client
  client1.cpp
  receiver.cpp
  decoder.cpp
)

TARGET_LINK_LIBRARIES(publish_video_client
  flow
  opencv_imgproc
  opencv_core
  opencv_highgui
  pthread
)
