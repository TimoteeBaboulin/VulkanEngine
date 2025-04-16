#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Quick3DAssetUtils" for configuration "Debug"
set_property(TARGET Qt6::Quick3DAssetUtils APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::Quick3DAssetUtils PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6Quick3DAssetUtilsd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "Qt6::Quick3DAssetImport;Qt6::Quick3DRuntimeRender;Qt6::Quick3D;Qt6::Qml;Qt6::QuickTimeline"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6Quick3DAssetUtilsd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Quick3DAssetUtils )
list(APPEND _cmake_import_check_files_for_Qt6::Quick3DAssetUtils "${_IMPORT_PREFIX}/lib/Qt6Quick3DAssetUtilsd.lib" "${_IMPORT_PREFIX}/bin/Qt6Quick3DAssetUtilsd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
