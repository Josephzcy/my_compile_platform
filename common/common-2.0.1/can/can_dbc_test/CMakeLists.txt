
INCLUDE_DIRECTORIES("../can_dbc")

# test
ADD_EXECUTABLE(can_dbc_test "./dbc_test.cpp")
TARGET_LINK_LIBRARIES(can_dbc_test can_dbc)
