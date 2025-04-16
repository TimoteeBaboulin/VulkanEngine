#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::ChartsQml" for configuration "Debug"
set_property(TARGET Qt6::ChartsQml APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::ChartsQml PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6ChartsQmld.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6ChartsQmld.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::ChartsQml )
list(APPEND _cmake_import_check_files_for_Qt6::ChartsQml "${_IMPORT_PREFIX}/lib/Qt6ChartsQmld.lib" "${_IMPORT_PREFIX}/bin/Qt6ChartsQmld.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
