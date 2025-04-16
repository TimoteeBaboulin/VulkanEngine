#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Location" for configuration "Debug"
set_property(TARGET Qt6::Location APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::Location PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6Locationd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "Qt6::Gui;Qt6::Quick;Qt6::Qml"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6Locationd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Location )
list(APPEND _cmake_import_check_files_for_Qt6::Location "${_IMPORT_PREFIX}/lib/Qt6Locationd.lib" "${_IMPORT_PREFIX}/bin/Qt6Locationd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
