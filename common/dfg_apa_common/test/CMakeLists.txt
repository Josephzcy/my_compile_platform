ADD_EXECUTABLE(mapping_test mapping_test.cpp)

IF(WITH_CV22_CHIP)
    TARGET_LINK_LIBRARIES(mapping_test
        ApaCommon
        ${GLOG_LIBS}
        gflags
        ${OPENCV_LIBS}
        ${COMMON_LIBS}
        rpc
        ambaipc
        )
ELSE()
    TARGET_LINK_LIBRARIES(mapping_test
        ApaCommon
        ${GLOG_LIBS}
        gflags
        ${OPENCV_LIBS}
        ${COMMON_LIBS}
        )
ENDIF()
