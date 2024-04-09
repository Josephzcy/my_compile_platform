IF(${MY_BUILD_PLATFORM} MATCHES "x1")
  SET(EXE_FILE "flowout-linux-arm")

  ADD_CUSTOM_COMMAND(
    OUTPUT ${EXE_FILE}
    COMMAND cd ${PROJECT_ROOT_PATH}/examples/flowout &&
            ./build_zynq.sh &&
            cp ${EXE_FILE} "${EXECUTABLE_OUTPUT_PATH}/${EXE_FILE}"
    COMMENT "Build ${EXE_FILE}"
  )

  ADD_CUSTOM_TARGET(flowout ALL DEPENDS ${EXE_FILE})
ELSEIF(${MY_BUILD_PLATFORM} MATCHES "m4")
  SET(EXE_FILE "flowout-android-arm64")

  ADD_CUSTOM_COMMAND(
    OUTPUT ${EXE_FILE}
    COMMAND cd ${PROJECT_ROOT_PATH}/examples/flowout &&
            ./build_android.sh &&
            cp ${EXE_FILE} "${EXECUTABLE_OUTPUT_PATH}/${EXE_FILE}"
    COMMENT "Build ${EXE_FILE}"
  )

  ADD_CUSTOM_TARGET(Test1 ALL DEPENDS ${EXE_FILE})
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET(EXE_FILE "flowout-linux-amd64")

  ADD_CUSTOM_COMMAND(
    OUTPUT ${EXE_FILE}
    COMMAND cd ${PROJECT_ROOT_PATH}/examples/flowout &&
            ./build_linux.sh &&
            cp ${EXE_FILE} "${EXECUTABLE_OUTPUT_PATH}/${EXE_FILE}"
    COMMENT "Build ${EXE_FILE}"
  )

  ADD_CUSTOM_TARGET(flowout ALL DEPENDS ${EXE_FILE})
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(EXE_FILE "flowout-darwin-amd64")

  ADD_CUSTOM_COMMAND(
    OUTPUT ${EXE_FILE}
    COMMAND cd ${PROJECT_ROOT_PATH}/examples/flowout &&
            ./build_darwin.sh &&
            cp ${EXE_FILE} "${EXECUTABLE_OUTPUT_PATH}/${EXE_FILE}"
    COMMENT "Build ${EXE_FILE}"
  )

  ADD_CUSTOM_TARGET(flowout ALL DEPENDS ${EXE_FILE})
ENDIF()
