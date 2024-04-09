AUX_SOURCE_DIRECTORY(. SRC_LIST)

LIST(REMOVE_ITEM SRC_LIST "./polyfit_test.cpp")
#MESSAGE(STATUS ${SRC_LIST})

IF (WIN32)
    ADD_LIBRARY(math ${LIBS_TYPE} ${SRC_LIST} ${COMPATIBLE_SRC_LIST})
ELSEIF(ANDROID_ABI)
    ADD_LIBRARY(math ${LIBS_TYPE} ${SRC_LIST})
ELSE ()
    ADD_LIBRARY(math ${LIBS_TYPE} ${SRC_LIST})

    # test
#    ADD_EXECUTABLE(polyfit_test polyfit_test.cpp)
#    TARGET_LINK_LIBRARIES(polyfit_test math gtest gtest_main 
#        opencv_imgcodecs pthread opencv_core opencv_highgui opencv_imgproc)
ENDIF ()

INCLUDE_DIRECTORIES(${PROJECT_ROOT_PATH})
