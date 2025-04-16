#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::qquick3dplugin" for configuration "Debug"
set_property(TARGET Qt6::qquick3dplugin APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::qquick3dplugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/qml/QtQuick3D/qquick3dplugind.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::qquick3dplugin )
list(APPEND _cmake_import_check_files_for_Qt6::qquick3dplugin "${_IMPORT_PREFIX}/qml/QtQuick3D/qquick3dplugind.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
