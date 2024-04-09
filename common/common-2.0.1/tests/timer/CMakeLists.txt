IF (ANDROID_ABI)
ELSEIF (UNIX)
    ADD_EXECUTABLE(timer_manager_test "./timer_manager_test.cpp")
    TARGET_LINK_LIBRARIES(timer_manager_test
        timer
        pthread
        concurrency
        time
        gtest
        gtest_main
        ${Glog_LIBS})
ENDIF()

