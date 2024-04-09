AUX_SOURCE_DIRECTORY(. SRC_LIST)

LIST(REMOVE_ITEM SRC_LIST "./module_parallel_test.cpp")
#MESSAGE(STATUS ${SRC_LIST})

ADD_LIBRARY(concurrency ${LIBS_TYPE} ${SRC_LIST})
#ADD_LIBRARY(concurrency_shared SHARED ${SRC_LIST})
#SET_TARGET_PROPERTIES(concurrency_shared PROPERTIES OUTPUT_NAME "concurrency")

IF (ANDROID_ABI)
    TARGET_LINK_LIBRARIES(concurrency ${OpenCV_LIBS} base time system ${Glog_LIBS} log)
ELSEIF(MSVC)
    TARGET_LINK_LIBRARIES(concurrency ${OpenCV_LIBS} pthreadVC2 base time system)
ELSE()
    ADD_EXECUTABLE(thread_pool_test thread_pool_test.cpp)
    TARGET_LINK_LIBRARIES(concurrency ${OpenCV_LIBS} pthread base time system ${Glog_LIBS})
    TARGET_LINK_LIBRARIES(thread_pool_test
        base
        concurrency)
ENDIF ()


INCLUDE_DIRECTORIES(${PROJECT_ROOT_PATH})
