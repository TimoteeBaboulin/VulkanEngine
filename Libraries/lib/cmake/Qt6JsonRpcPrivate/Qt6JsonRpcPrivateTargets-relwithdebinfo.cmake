#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::JsonRpcPrivate" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::JsonRpcPrivate APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::JsonRpcPrivate PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6JsonRpc.lib"
  )

list(APPEND _cmake_import_check_targets Qt6::JsonRpcPrivate )
list(APPEND _cmake_import_check_files_for_Qt6::JsonRpcPrivate "${_IMPORT_PREFIX}/lib/Qt6JsonRpc.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
