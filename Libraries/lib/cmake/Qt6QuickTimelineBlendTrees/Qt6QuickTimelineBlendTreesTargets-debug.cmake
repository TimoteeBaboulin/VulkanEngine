#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QuickTimelineBlendTrees" for configuration "Debug"
set_property(TARGET Qt6::QuickTimelineBlendTrees APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::QuickTimelineBlendTrees PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6QuickTimelineBlendTreesd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6QuickTimelineBlendTreesd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::QuickTimelineBlendTrees )
list(APPEND _cmake_import_check_files_for_Qt6::QuickTimelineBlendTrees "${_IMPORT_PREFIX}/lib/Qt6QuickTimelineBlendTreesd.lib" "${_IMPORT_PREFIX}/bin/Qt6QuickTimelineBlendTreesd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
