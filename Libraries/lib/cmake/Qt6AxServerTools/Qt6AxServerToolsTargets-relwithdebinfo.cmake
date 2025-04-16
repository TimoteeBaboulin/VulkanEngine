#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::idc" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::idc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::idc PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/idc.exe"
  )

list(APPEND _cmake_import_check_targets Qt6::idc )
list(APPEND _cmake_import_check_files_for_Qt6::idc "${_IMPORT_PREFIX}/bin/idc.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
