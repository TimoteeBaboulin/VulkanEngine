#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DCore" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::3DCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::3DCore PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt63DCore.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt63DCore.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DCore )
list(APPEND _cmake_import_check_files_for_Qt6::3DCore "${_IMPORT_PREFIX}/lib/Qt63DCore.lib" "${_IMPORT_PREFIX}/bin/Qt63DCore.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
