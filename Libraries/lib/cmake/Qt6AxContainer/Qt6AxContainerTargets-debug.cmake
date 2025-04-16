#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::AxContainer" for configuration "Debug"
set_property(TARGET Qt6::AxContainer APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::AxContainer PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/Qt6AxContainerd.lib"
  )

list(APPEND _cmake_import_check_targets Qt6::AxContainer )
list(APPEND _cmake_import_check_files_for_Qt6::AxContainer "${_IMPORT_PREFIX}/lib/Qt6AxContainerd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
