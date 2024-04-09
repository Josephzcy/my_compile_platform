IF (ANDROID_ABI)
ELSEIF (UNIX)
    ADD_EXECUTABLE(producer_consumer_queue_test "./producer_consumer_queue_test.cpp")
    TARGET_LINK_LIBRARIES(producer_consumer_queue_test 
        pthread 
        memory 
        concurrency 
        gtest 
        gtest_main 
        ${Glog_LIBS})
ENDIF()

