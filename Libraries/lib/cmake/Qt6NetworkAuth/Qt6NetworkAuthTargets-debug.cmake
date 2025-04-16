#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::NetworkAuth" for configuration "Debug"
set_property(TARGET Qt6::NetworkAuth APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::NetworkAuth PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6NetworkAuthd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "Qt6::Gui"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6NetworkAuthd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::NetworkAuth )
list(APPEND _cmake_import_check_files_for_Qt6::NetworkAuth "${_IMPORT_PREFIX}/lib/Qt6NetworkAuthd.lib" "${_IMPORT_PREFIX}/bin/Qt6NetworkAuthd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
