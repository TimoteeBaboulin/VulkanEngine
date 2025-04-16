#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QuickTimelineBlendTrees" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::QuickTimelineBlendTrees APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::QuickTimelineBlendTrees PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6QuickTimelineBlendTrees.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6QuickTimelineBlendTrees.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::QuickTimelineBlendTrees )
list(APPEND _cmake_import_check_files_for_Qt6::QuickTimelineBlendTrees "${_IMPORT_PREFIX}/lib/Qt6QuickTimelineBlendTrees.lib" "${_IMPORT_PREFIX}/bin/Qt6QuickTimelineBlendTrees.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
