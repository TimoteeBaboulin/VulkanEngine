#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Charts" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::Charts APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::Charts PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6Charts.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6Charts.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Charts )
list(APPEND _cmake_import_check_files_for_Qt6::Charts "${_IMPORT_PREFIX}/lib/Qt6Charts.lib" "${_IMPORT_PREFIX}/bin/Qt6Charts.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
