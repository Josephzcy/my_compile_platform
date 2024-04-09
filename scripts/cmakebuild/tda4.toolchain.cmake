# CMake system name must be something like "Linux".
# This is important for cross-compiling.

# usage:
# -DCMAKE_TOOLCHAIN_FILE=path/to/this/file

set(TOOLCHAIN_PATH /opt/toolchains/tda4/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin)
set( CMAKE_SYSTEM_NAME Linux )
set( CMAKE_SYSTEM_PROCESSOR aarch64 )
set( CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-gcc )
set( CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-g++ )
set( CMAKE_C_COMPILER_LAUNCHER  )
set( CMAKE_CXX_COMPILER_LAUNCHER  )
set( CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-gcc )
find_program( CMAKE_AR ${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-gcc-ar DOC "Archiver" REQUIRED )

set( SYSROOT $ENV{SYSROOT} )

set( CMAKE_C_FLAGS "  --sysroot=${SYSROOT}  -O2 -pipe -g -feliminate-unused-debug-types " CACHE STRING "CFLAGS" )
set( CMAKE_CXX_FLAGS "  --sysroot=${SYSROOT}  -O2 -pipe -g -feliminate-unused-debug-types " CACHE STRING "CXXFLAGS" )
set( CMAKE_ASM_FLAGS "  --sysroot=${SYSROOT}  -O2 -pipe -g -feliminate-unused-debug-types " CACHE STRING "ASM FLAGS" )

set( CMAKE_C_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CFLAGS for release" )
set( CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CXXFLAGS for release" )
set( CMAKE_ASM_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional ASM FLAGS for release" )

set( CMAKE_C_LINK_FLAGS "  --sysroot=${SYSROOT} -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed" CACHE STRING "LDFLAGS" )
set( CMAKE_CXX_LINK_FLAGS "  --sysroot=${SYSROOT} -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed" CACHE STRING "LDFLAGS" )

# only search in the paths provided so cmake doesnt pick
# up libraries and tools from the native build machine
set( CMAKE_FIND_ROOT_PATH ${SYSROOT} )
set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
set( CMAKE_PROGRAM_PATH "/" )

# We need to set the rpath to the correct directory as cmake does not provide any
# directory as rpath by default
set( CMAKE_INSTALL_RPATH  )

# Use RPATHs relative to build directory for reproducibility
set( CMAKE_BUILD_RPATH_USE_ORIGIN ON )

# Use our cmake modules
list(APPEND CMAKE_MODULE_PATH "${SYSROOT}/usr/share/cmake/Modules/")

# add for non /usr/lib libdir, e.g. /usr/lib64
# set( CMAKE_LIBRARY_PATH /usr/lib /lib)

# add include dir to implicit includes in case it differs from /usr/include
# list(APPEND CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES /usr/include)
# list(APPEND CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES /usr/include)

