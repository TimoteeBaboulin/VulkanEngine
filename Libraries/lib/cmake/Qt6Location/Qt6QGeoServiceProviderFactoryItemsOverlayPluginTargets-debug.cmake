#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QGeoServiceProviderFactoryItemsOverlayPlugin" for configuration "Debug"
set_property(TARGET Qt6::QGeoServiceProviderFactoryItemsOverlayPlugin APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::QGeoServiceProviderFactoryItemsOverlayPlugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/plugins/geoservices/qtgeoservices_itemsoverlayd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::QGeoServiceProviderFactoryItemsOverlayPlugin )
list(APPEND _cmake_import_check_files_for_Qt6::QGeoServiceProviderFactoryItemsOverlayPlugin "${_IMPORT_PREFIX}/plugins/geoservices/qtgeoservices_itemsoverlayd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
