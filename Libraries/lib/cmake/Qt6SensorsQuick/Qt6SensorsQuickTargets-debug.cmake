#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::SensorsQuick" for configuration "Debug"
set_property(TARGET Qt6::SensorsQuick APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::SensorsQuick PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6SensorsQuickd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6SensorsQuickd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::SensorsQuick )
list(APPEND _cmake_import_check_files_for_Qt6::SensorsQuick "${_IMPORT_PREFIX}/lib/Qt6SensorsQuickd.lib" "${_IMPORT_PREFIX}/bin/Qt6SensorsQuickd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
