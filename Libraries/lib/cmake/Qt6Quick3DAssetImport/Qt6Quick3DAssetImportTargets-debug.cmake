#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Quick3DAssetImport" for configuration "Debug"
set_property(TARGET Qt6::Quick3DAssetImport APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::Quick3DAssetImport PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6Quick3DAssetImportd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6Quick3DAssetImportd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Quick3DAssetImport )
list(APPEND _cmake_import_check_files_for_Qt6::Quick3DAssetImport "${_IMPORT_PREFIX}/lib/Qt6Quick3DAssetImportd.lib" "${_IMPORT_PREFIX}/bin/Qt6Quick3DAssetImportd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
