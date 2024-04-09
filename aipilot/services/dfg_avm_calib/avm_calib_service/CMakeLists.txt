
# configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/../include/version.h)
AUX_SOURCE_DIRECTORY(. SRC_LIST)

SET(SYSROOT /opt/toolchains/tda4/sysroots/)
SET(SDK_PATH ${SYSROOT}/usr/include/processor_sdk/)
message(STATUS "sdk_path:${SDK_PATH}")
INCLUDE_DIRECTORIES(${SDK_PATH}/tidl_j7_02_00_00_07/ti_dl/inc)
INCLUDE_DIRECTORIES(${SDK_PATH}/ivision)
INCLUDE_DIRECTORIES(${SDK_PATH}/tiovx/include)
INCLUDE_DIRECTORIES(${SDK_PATH}/tiovx/kernels_j7/include)
INCLUDE_DIRECTORIES(${SDK_PATH}/tiovx/kernels/include)
INCLUDE_DIRECTORIES(${SDK_PATH}/vision_apps)
INCLUDE_DIRECTORIES(${SDK_PATH}/vision_apps/apps/basic_demos/app_tirtos/common)
INCLUDE_DIRECTORIES(${SDK_PATH}/vision_apps/apps/basic_demos/app_tirtos/tirtos_linux)

INCLUDE_DIRECTORIES(${SYSROOT}/usr/include)
LINK_DIRECTORIES(${SYSROOT}/usr/lib)
LINK_DIRECTORIES(${SYSROOT}/lib)


IF (BUILD_SHARED_LIBS)
    add_library(avm_service SHARED ${SRC_LIST} ${PB_FILE})
ELSE ()
    add_library(avm_service STATIC ${SRC_LIST} ${PB_FILE})
ENDIF ()
target_link_libraries(avm_service
        dds_api
        dds
        tbb
        z
        rt
        dl
        pthread
        DiagAgent
        tivision_apps
        avm_calib
        dfg_avm_mapping
        )
