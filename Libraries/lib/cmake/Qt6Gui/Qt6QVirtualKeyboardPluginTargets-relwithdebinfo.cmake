#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QVirtualKeyboardPlugin" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::QVirtualKeyboardPlugin APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::QVirtualKeyboardPlugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELWITHDEBINFO ""
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/plugins/platforminputcontexts/qtvirtualkeyboardplugin.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::QVirtualKeyboardPlugin )
list(APPEND _cmake_import_check_files_for_Qt6::QVirtualKeyboardPlugin "${_IMPORT_PREFIX}/plugins/platforminputcontexts/qtvirtualkeyboardplugin.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
