#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::qtvkbplugin" for configuration "Debug"
set_property(TARGET Qt6::qtvkbplugin APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::qtvkbplugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/qml/QtQuick/VirtualKeyboard/qtvkbplugind.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::qtvkbplugin )
list(APPEND _cmake_import_check_files_for_Qt6::qtvkbplugin "${_IMPORT_PREFIX}/qml/QtQuick/VirtualKeyboard/qtvkbplugind.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
