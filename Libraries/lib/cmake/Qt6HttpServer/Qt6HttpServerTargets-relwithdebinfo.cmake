#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::HttpServer" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::HttpServer APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::HttpServer PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6HttpServer.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6HttpServer.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::HttpServer )
list(APPEND _cmake_import_check_files_for_Qt6::HttpServer "${_IMPORT_PREFIX}/lib/Qt6HttpServer.lib" "${_IMPORT_PREFIX}/bin/Qt6HttpServer.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
