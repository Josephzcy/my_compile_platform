
AUX_SOURCE_DIRECTORY(. SRC_LIST)

IF (BUILD_SHARED_LIBS)
    add_library(avm_calib SHARED ${SRC_LIST} ${PB_FILE})
ELSE ()
    add_library(avm_calib STATIC ${SRC_LIST} ${PB_FILE})
ENDIF ()

target_link_libraries(avm_calib dfg_avm_mapping)
