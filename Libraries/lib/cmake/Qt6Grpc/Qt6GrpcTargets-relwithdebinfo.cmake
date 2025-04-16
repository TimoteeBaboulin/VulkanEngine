#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Grpc" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::Grpc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::Grpc PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6Grpc.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6Grpc.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Grpc )
list(APPEND _cmake_import_check_files_for_Qt6::Grpc "${_IMPORT_PREFIX}/lib/Qt6Grpc.lib" "${_IMPORT_PREFIX}/bin/Qt6Grpc.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
