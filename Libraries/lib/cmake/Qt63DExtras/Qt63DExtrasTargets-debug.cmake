#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DExtras" for configuration "Debug"
set_property(TARGET Qt6::3DExtras APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::3DExtras PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt63DExtrasd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt63DExtrasd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DExtras )
list(APPEND _cmake_import_check_files_for_Qt6::3DExtras "${_IMPORT_PREFIX}/lib/Qt63DExtrasd.lib" "${_IMPORT_PREFIX}/bin/Qt63DExtrasd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
