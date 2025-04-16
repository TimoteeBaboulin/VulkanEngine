#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::WebSockets" for configuration "Debug"
set_property(TARGET Qt6::WebSockets APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::WebSockets PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6WebSocketsd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6WebSocketsd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::WebSockets )
list(APPEND _cmake_import_check_files_for_Qt6::WebSockets "${_IMPORT_PREFIX}/lib/Qt6WebSocketsd.lib" "${_IMPORT_PREFIX}/bin/Qt6WebSocketsd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
