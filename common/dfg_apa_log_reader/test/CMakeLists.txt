CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(add_test)

INCLUDE_DIRECTORIES(
    ../include/apa_log_read
    )

ADD_EXECUTABLE(read_json_log_test read_json_log_test.cpp)

TARGET_LINK_LIBRARIES(read_json_log_test
    ApaJsonReader
    pthread
    shared_env
    glog
    gflags
    ${OPENCV_LIBS}
    )

ADD_EXECUTABLE(read_log_test read_log_test.cpp)

TARGET_LINK_LIBRARIES(read_log_test
    ApaLogReader
    pthread
    shared_env
    glog
    gflags
    ${OPENCV_LIBS}
    )
    
ADD_EXECUTABLE(read_sim_log_test read_sim_log_test.cpp)

TARGET_LINK_LIBRARIES(read_sim_log_test
    ApaSimLogReader
    pthread
    shared_env
    glog
    gflags
    ${OPENCV_LIBS}
    )