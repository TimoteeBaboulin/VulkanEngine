#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::RemoteObjects" for configuration "Debug"
set_property(TARGET Qt6::RemoteObjects APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::RemoteObjects PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6RemoteObjectsd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6RemoteObjectsd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::RemoteObjects )
list(APPEND _cmake_import_check_files_for_Qt6::RemoteObjects "${_IMPORT_PREFIX}/lib/Qt6RemoteObjectsd.lib" "${_IMPORT_PREFIX}/bin/Qt6RemoteObjectsd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
