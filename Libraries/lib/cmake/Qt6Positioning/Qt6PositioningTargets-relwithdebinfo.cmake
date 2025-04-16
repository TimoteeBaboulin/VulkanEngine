#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Positioning" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::Positioning APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::Positioning PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6Positioning.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6Positioning.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Positioning )
list(APPEND _cmake_import_check_files_for_Qt6::Positioning "${_IMPORT_PREFIX}/lib/Qt6Positioning.lib" "${_IMPORT_PREFIX}/bin/Qt6Positioning.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
