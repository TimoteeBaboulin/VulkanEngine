#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Quick3DGlslParserPrivate" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::Quick3DGlslParserPrivate APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::Quick3DGlslParserPrivate PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6Quick3DGlslParser.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6Quick3DGlslParser.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Quick3DGlslParserPrivate )
list(APPEND _cmake_import_check_files_for_Qt6::Quick3DGlslParserPrivate "${_IMPORT_PREFIX}/lib/Qt6Quick3DGlslParser.lib" "${_IMPORT_PREFIX}/bin/Qt6Quick3DGlslParser.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
