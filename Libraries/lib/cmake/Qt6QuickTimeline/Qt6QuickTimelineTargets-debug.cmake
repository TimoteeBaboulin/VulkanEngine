#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QuickTimeline" for configuration "Debug"
set_property(TARGET Qt6::QuickTimeline APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::QuickTimeline PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6QuickTimelined.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6QuickTimelined.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::QuickTimeline )
list(APPEND _cmake_import_check_files_for_Qt6::QuickTimeline "${_IMPORT_PREFIX}/lib/Qt6QuickTimelined.lib" "${_IMPORT_PREFIX}/bin/Qt6QuickTimelined.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
