#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::VirtualKeyboard" for configuration "Debug"
set_property(TARGET Qt6::VirtualKeyboard APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::VirtualKeyboard PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6VirtualKeyboardd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6VirtualKeyboardd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::VirtualKeyboard )
list(APPEND _cmake_import_check_files_for_Qt6::VirtualKeyboard "${_IMPORT_PREFIX}/lib/Qt6VirtualKeyboardd.lib" "${_IMPORT_PREFIX}/bin/Qt6VirtualKeyboardd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
