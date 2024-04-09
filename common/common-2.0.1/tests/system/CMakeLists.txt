AUX_SOURCE_DIRECTORY(. SRC_LIST)

ADD_EXECUTABLE(test_common_system ${SRC_LIST})
SET_TARGET_PROPERTIES(test_common_system PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TESTS_OUTPUT_PATH}")
TARGET_LINK_LIBRARIES(test_common_system pthread concurrency system gtest)

ADD_TEST(NAME test_common_system COMMAND test_common_system)
