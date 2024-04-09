#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Genivi::dlt" for configuration "RelWithDebInfo"
set_property(TARGET Genivi::dlt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Genivi::dlt PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libdlt.so.2.18.8"
  IMPORTED_SONAME_RELWITHDEBINFO "libdlt.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS Genivi::dlt )
list(APPEND _IMPORT_CHECK_FILES_FOR_Genivi::dlt "${_IMPORT_PREFIX}/lib/libdlt.so.2.18.8" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
