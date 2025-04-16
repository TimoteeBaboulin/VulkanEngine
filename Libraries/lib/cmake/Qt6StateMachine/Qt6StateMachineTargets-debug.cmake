#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::StateMachine" for configuration "Debug"
set_property(TARGET Qt6::StateMachine APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::StateMachine PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6StateMachined.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6StateMachined.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::StateMachine )
list(APPEND _cmake_import_check_files_for_Qt6::StateMachine "${_IMPORT_PREFIX}/lib/Qt6StateMachined.lib" "${_IMPORT_PREFIX}/bin/Qt6StateMachined.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
