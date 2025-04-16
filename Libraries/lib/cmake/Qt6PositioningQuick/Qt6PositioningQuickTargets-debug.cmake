#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::PositioningQuick" for configuration "Debug"
set_property(TARGET Qt6::PositioningQuick APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::PositioningQuick PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6PositioningQuickd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6PositioningQuickd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::PositioningQuick )
list(APPEND _cmake_import_check_files_for_Qt6::PositioningQuick "${_IMPORT_PREFIX}/lib/Qt6PositioningQuickd.lib" "${_IMPORT_PREFIX}/bin/Qt6PositioningQuickd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
