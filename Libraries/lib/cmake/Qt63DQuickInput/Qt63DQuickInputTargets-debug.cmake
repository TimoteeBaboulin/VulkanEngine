#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DQuickInput" for configuration "Debug"
set_property(TARGET Qt6::3DQuickInput APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::3DQuickInput PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt63DQuickInputd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt63DQuickInputd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DQuickInput )
list(APPEND _cmake_import_check_files_for_Qt6::3DQuickInput "${_IMPORT_PREFIX}/lib/Qt63DQuickInputd.lib" "${_IMPORT_PREFIX}/bin/Qt63DQuickInputd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
