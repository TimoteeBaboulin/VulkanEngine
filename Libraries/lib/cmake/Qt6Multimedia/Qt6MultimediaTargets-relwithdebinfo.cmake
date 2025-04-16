#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Multimedia" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::Multimedia APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::Multimedia PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6Multimedia.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6Multimedia.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Multimedia )
list(APPEND _cmake_import_check_files_for_Qt6::Multimedia "${_IMPORT_PREFIX}/lib/Qt6Multimedia.lib" "${_IMPORT_PREFIX}/bin/Qt6Multimedia.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
