#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DQuickRender" for configuration "Debug"
set_property(TARGET Qt6::3DQuickRender APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::3DQuickRender PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt63DQuickRenderd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt63DQuickRenderd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DQuickRender )
list(APPEND _cmake_import_check_files_for_Qt6::3DQuickRender "${_IMPORT_PREFIX}/lib/Qt63DQuickRenderd.lib" "${_IMPORT_PREFIX}/bin/Qt63DQuickRenderd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
