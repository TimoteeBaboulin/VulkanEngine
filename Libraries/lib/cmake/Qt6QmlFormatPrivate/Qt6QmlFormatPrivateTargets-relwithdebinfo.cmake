#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QmlFormatPrivate" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::QmlFormatPrivate APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::QmlFormatPrivate PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6QmlFormat.lib"
  )

list(APPEND _cmake_import_check_targets Qt6::QmlFormatPrivate )
list(APPEND _cmake_import_check_files_for_Qt6::QmlFormatPrivate "${_IMPORT_PREFIX}/lib/Qt6QmlFormat.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
