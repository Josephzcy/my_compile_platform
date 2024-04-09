AUX_SOURCE_DIRECTORY(. SRC_LIST)

ADD_EXECUTABLE(test_common_time ${SRC_LIST})
SET_TARGET_PROPERTIES(test_common_time PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TESTS_OUTPUT_PATH}")
TARGET_LINK_LIBRARIES(test_common_time pthread time concurrency gtest)

ADD_TEST(NAME test_common_time COMMAND test_common_time)
