#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::FFmpegMediaPluginImplPrivate" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::FFmpegMediaPluginImplPrivate APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::FFmpegMediaPluginImplPrivate PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6FFmpegMediaPluginImpl.lib"
  )

list(APPEND _cmake_import_check_targets Qt6::FFmpegMediaPluginImplPrivate )
list(APPEND _cmake_import_check_files_for_Qt6::FFmpegMediaPluginImplPrivate "${_IMPORT_PREFIX}/lib/Qt6FFmpegMediaPluginImpl.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
