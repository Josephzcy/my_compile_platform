AUX_SOURCE_DIRECTORY(. SRC_LIST)

ADD_EXECUTABLE(test_common_memory ${SRC_LIST})
SET_TARGET_PROPERTIES(test_common_memory PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TESTS_OUTPUT_PATH}")
TARGET_LINK_LIBRARIES(test_common_memory pthread memory concurrency gtest ${Glog_LIBS})

ADD_TEST(NAME test_common_memory COMMAND test_common_memory)
