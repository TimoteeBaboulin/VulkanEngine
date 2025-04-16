#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::WebView" for configuration "Debug"
set_property(TARGET Qt6::WebView APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::WebView PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6WebViewd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6WebViewd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::WebView )
list(APPEND _cmake_import_check_files_for_Qt6::WebView "${_IMPORT_PREFIX}/lib/Qt6WebViewd.lib" "${_IMPORT_PREFIX}/bin/Qt6WebViewd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
