#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::JsonRpcPrivate" for configuration "Debug"
set_property(TARGET Qt6::JsonRpcPrivate APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::JsonRpcPrivate PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/Qt6JsonRpcd.lib"
  )

list(APPEND _cmake_import_check_targets Qt6::JsonRpcPrivate )
list(APPEND _cmake_import_check_files_for_Qt6::JsonRpcPrivate "${_IMPORT_PREFIX}/lib/Qt6JsonRpcd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
