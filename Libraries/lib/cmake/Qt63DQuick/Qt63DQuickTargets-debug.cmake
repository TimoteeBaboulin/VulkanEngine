#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DQuick" for configuration "Debug"
set_property(TARGET Qt6::3DQuick APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::3DQuick PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt63DQuickd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt63DQuickd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DQuick )
list(APPEND _cmake_import_check_files_for_Qt6::3DQuick "${_IMPORT_PREFIX}/lib/Qt63DQuickd.lib" "${_IMPORT_PREFIX}/bin/Qt63DQuickd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
