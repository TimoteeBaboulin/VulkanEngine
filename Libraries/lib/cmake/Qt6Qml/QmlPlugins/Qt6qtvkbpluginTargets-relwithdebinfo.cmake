#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::qtvkbplugin" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::qtvkbplugin APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::qtvkbplugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELWITHDEBINFO ""
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/qml/QtQuick/VirtualKeyboard/qtvkbplugin.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::qtvkbplugin )
list(APPEND _cmake_import_check_files_for_Qt6::qtvkbplugin "${_IMPORT_PREFIX}/qml/QtQuick/VirtualKeyboard/qtvkbplugin.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
