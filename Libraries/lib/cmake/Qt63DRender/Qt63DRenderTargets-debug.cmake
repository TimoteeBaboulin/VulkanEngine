#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DRender" for configuration "Debug"
set_property(TARGET Qt6::3DRender APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::3DRender PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt63DRenderd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "Qt6::Concurrent"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt63DRenderd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DRender )
list(APPEND _cmake_import_check_files_for_Qt6::3DRender "${_IMPORT_PREFIX}/lib/Qt63DRenderd.lib" "${_IMPORT_PREFIX}/bin/Qt63DRenderd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
