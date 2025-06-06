#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DRender" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::3DRender APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::3DRender PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt63DRender.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "Qt6::Concurrent"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt63DRender.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DRender )
list(APPEND _cmake_import_check_files_for_Qt6::3DRender "${_IMPORT_PREFIX}/lib/Qt63DRender.lib" "${_IMPORT_PREFIX}/bin/Qt63DRender.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
