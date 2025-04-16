#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Nfc" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::Nfc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::Nfc PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6Nfc.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6Nfc.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Nfc )
list(APPEND _cmake_import_check_files_for_Qt6::Nfc "${_IMPORT_PREFIX}/lib/Qt6Nfc.lib" "${_IMPORT_PREFIX}/bin/Qt6Nfc.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
