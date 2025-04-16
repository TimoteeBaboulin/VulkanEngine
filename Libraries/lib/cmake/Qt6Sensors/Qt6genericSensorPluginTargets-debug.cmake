#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::genericSensorPlugin" for configuration "Debug"
set_property(TARGET Qt6::genericSensorPlugin APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::genericSensorPlugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/plugins/sensors/qtsensors_genericd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::genericSensorPlugin )
list(APPEND _cmake_import_check_files_for_Qt6::genericSensorPlugin "${_IMPORT_PREFIX}/plugins/sensors/qtsensors_genericd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
