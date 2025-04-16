#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DQuickInput" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::3DQuickInput APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::3DQuickInput PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt63DQuickInput.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt63DQuickInput.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DQuickInput )
list(APPEND _cmake_import_check_files_for_Qt6::3DQuickInput "${_IMPORT_PREFIX}/lib/Qt63DQuickInput.lib" "${_IMPORT_PREFIX}/bin/Qt63DQuickInput.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
