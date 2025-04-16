#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::SerialPort" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::SerialPort APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::SerialPort PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6SerialPort.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6SerialPort.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::SerialPort )
list(APPEND _cmake_import_check_files_for_Qt6::SerialPort "${_IMPORT_PREFIX}/lib/Qt6SerialPort.lib" "${_IMPORT_PREFIX}/bin/Qt6SerialPort.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
