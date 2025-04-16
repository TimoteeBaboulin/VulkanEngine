#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::ProtobufQtGuiTypes" for configuration "Debug"
set_property(TARGET Qt6::ProtobufQtGuiTypes APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::ProtobufQtGuiTypes PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6ProtobufQtGuiTypesd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "Qt6::Core;Qt6::Gui;Qt6::Protobuf"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6ProtobufQtGuiTypesd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::ProtobufQtGuiTypes )
list(APPEND _cmake_import_check_files_for_Qt6::ProtobufQtGuiTypes "${_IMPORT_PREFIX}/lib/Qt6ProtobufQtGuiTypesd.lib" "${_IMPORT_PREFIX}/bin/Qt6ProtobufQtGuiTypesd.dll" )

# Import target "Qt6::ProtobufQtGuiTypes_qtprotoreg" for configuration "Debug"
set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::ProtobufQtGuiTypes_qtprotoreg PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_OBJECTS_DEBUG "${_IMPORT_PREFIX}/lib/objects-Debug/ProtobufQtGuiTypes_qtprotoreg/QtGui/QtGui_qtprotoreg.cpp.obj"
  )

list(APPEND _cmake_import_check_targets Qt6::ProtobufQtGuiTypes_qtprotoreg )
list(APPEND _cmake_import_check_files_for_Qt6::ProtobufQtGuiTypes_qtprotoreg "${_IMPORT_PREFIX}/lib/objects-Debug/ProtobufQtGuiTypes_qtprotoreg/QtGui/QtGui_qtprotoreg.cpp.obj" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
