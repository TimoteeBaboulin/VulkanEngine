#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DQuickExtras" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::3DQuickExtras APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::3DQuickExtras PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt63DQuickExtras.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt63DQuickExtras.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DQuickExtras )
list(APPEND _cmake_import_check_files_for_Qt6::3DQuickExtras "${_IMPORT_PREFIX}/lib/Qt63DQuickExtras.lib" "${_IMPORT_PREFIX}/bin/Qt63DQuickExtras.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
