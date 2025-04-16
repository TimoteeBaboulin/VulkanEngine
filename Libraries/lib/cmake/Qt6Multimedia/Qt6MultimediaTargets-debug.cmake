#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Multimedia" for configuration "Debug"
set_property(TARGET Qt6::Multimedia APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::Multimedia PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6Multimediad.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6Multimediad.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Multimedia )
list(APPEND _cmake_import_check_files_for_Qt6::Multimedia "${_IMPORT_PREFIX}/lib/Qt6Multimediad.lib" "${_IMPORT_PREFIX}/bin/Qt6Multimediad.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
