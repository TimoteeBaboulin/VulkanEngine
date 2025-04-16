#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::qtvkbbuiltinstylesplugin" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::qtvkbbuiltinstylesplugin APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::qtvkbbuiltinstylesplugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELWITHDEBINFO ""
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/qml/QtQuick/VirtualKeyboard/Styles/Builtin/qtvkbbuiltinstylesplugin.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::qtvkbbuiltinstylesplugin )
list(APPEND _cmake_import_check_files_for_Qt6::qtvkbbuiltinstylesplugin "${_IMPORT_PREFIX}/qml/QtQuick/VirtualKeyboard/Styles/Builtin/qtvkbbuiltinstylesplugin.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
