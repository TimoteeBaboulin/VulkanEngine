#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::dumpdoc" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::dumpdoc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::dumpdoc PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/dumpdoc.exe"
  )

list(APPEND _cmake_import_check_targets Qt6::dumpdoc )
list(APPEND _cmake_import_check_files_for_Qt6::dumpdoc "${_IMPORT_PREFIX}/bin/dumpdoc.exe" )

# Import target "Qt6::dumpcpp" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::dumpcpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::dumpcpp PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/dumpcpp.exe"
  )

list(APPEND _cmake_import_check_targets Qt6::dumpcpp )
list(APPEND _cmake_import_check_files_for_Qt6::dumpcpp "${_IMPORT_PREFIX}/bin/dumpcpp.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
