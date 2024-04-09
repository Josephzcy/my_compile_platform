

if (${CMAKE_SYSTEM_NAME} STREQUAL "QNX")
  set(EXAMPLES_LINK_LIBS socket)
endif()

#=======================================
ADD_SUBDIRECTORY(flowout)

#=======================================
ADD_EXECUTABLE(test1 test1.cpp)
TARGET_LINK_LIBRARIES(test1 flow ${EXAMPLES_LINK_LIBS})

#=======================================
ADD_EXECUTABLE(test2 test2.cpp)
TARGET_LINK_LIBRARIES(test2 flow ${EXAMPLES_LINK_LIBS})

#=======================================
ADD_EXECUTABLE(test3 test3.cpp)
TARGET_LINK_LIBRARIES(test3 flow)

#=======================================
ADD_EXECUTABLE(send_can_frames send_can_frames.cpp)
TARGET_LINK_LIBRARIES(send_can_frames flow ${EXAMPLES_LINK_LIBS})

#=======================================
ADD_EXECUTABLE(http_server http_server.cpp)
TARGET_LINK_LIBRARIES(http_server flow ${EXAMPLES_LINK_LIBS})

#=======================================
ADD_EXECUTABLE(client1 client1.cpp)
TARGET_LINK_LIBRARIES(client1 flow ${EXAMPLES_LINK_LIBS})

#=======================================
ADD_EXECUTABLE(client2 client2.cpp)
TARGET_LINK_LIBRARIES(client2 flow ${EXAMPLES_LINK_LIBS})

#=======================================
ADD_EXECUTABLE(server_client_test server_client_test.cpp)
TARGET_LINK_LIBRARIES(server_client_test flow ${EXAMPLES_LINK_LIBS})

#=======================================
# platform specific examples
IF(${MY_BUILD_PLATFORM} MATCHES "x1")
ELSEIF(${MY_BUILD_PLATFORM} MATCHES "m4")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  ADD_SUBDIRECTORY(publish_video)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  ADD_SUBDIRECTORY(publish_video)
ENDIF()
