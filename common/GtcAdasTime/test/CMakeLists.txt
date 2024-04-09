
AUX_SOURCE_DIRECTORY (. SRC_LIST)

add_executable(gtctimer_test ${SRC_LIST})
target_link_libraries(gtctimer_test gtctimer)
