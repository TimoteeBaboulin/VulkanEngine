#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::qtquickcontrols2plugin" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::qtquickcontrols2plugin APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::qtquickcontrols2plugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELWITHDEBINFO ""
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/qml/QtQuick/Controls/qtquickcontrols2plugin.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::qtquickcontrols2plugin )
list(APPEND _cmake_import_check_files_for_Qt6::qtquickcontrols2plugin "${_IMPORT_PREFIX}/qml/QtQuick/Controls/qtquickcontrols2plugin.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
