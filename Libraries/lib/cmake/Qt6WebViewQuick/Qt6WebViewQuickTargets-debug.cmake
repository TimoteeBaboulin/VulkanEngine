#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::WebViewQuick" for configuration "Debug"
set_property(TARGET Qt6::WebViewQuick APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::WebViewQuick PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6WebViewQuickd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "Qt6::WebView;Qt6::Qml"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6WebViewQuickd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::WebViewQuick )
list(APPEND _cmake_import_check_files_for_Qt6::WebViewQuick "${_IMPORT_PREFIX}/lib/Qt6WebViewQuickd.lib" "${_IMPORT_PREFIX}/bin/Qt6WebViewQuickd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
