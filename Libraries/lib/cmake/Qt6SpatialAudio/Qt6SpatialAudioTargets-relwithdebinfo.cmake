#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::SpatialAudio" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::SpatialAudio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::SpatialAudio PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6SpatialAudio.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6SpatialAudio.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::SpatialAudio )
list(APPEND _cmake_import_check_files_for_Qt6::SpatialAudio "${_IMPORT_PREFIX}/lib/Qt6SpatialAudio.lib" "${_IMPORT_PREFIX}/bin/Qt6SpatialAudio.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
