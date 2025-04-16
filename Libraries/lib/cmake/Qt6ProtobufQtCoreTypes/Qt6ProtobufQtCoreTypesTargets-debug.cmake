#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::ProtobufQtCoreTypes" for configuration "Debug"
set_property(TARGET Qt6::ProtobufQtCoreTypes APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::ProtobufQtCoreTypes PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/Qt6ProtobufQtCoreTypesd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "Qt6::Core;Qt6::Protobuf"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/Qt6ProtobufQtCoreTypesd.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::ProtobufQtCoreTypes )
list(APPEND _cmake_import_check_files_for_Qt6::ProtobufQtCoreTypes "${_IMPORT_PREFIX}/lib/Qt6ProtobufQtCoreTypesd.lib" "${_IMPORT_PREFIX}/bin/Qt6ProtobufQtCoreTypesd.dll" )

# Import target "Qt6::ProtobufQtCoreTypes_qtprotoreg" for configuration "Debug"
set_property(TARGET Qt6::ProtobufQtCoreTypes_qtprotoreg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Qt6::ProtobufQtCoreTypes_qtprotoreg PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_OBJECTS_DEBUG "${_IMPORT_PREFIX}/lib/objects-Debug/ProtobufQtCoreTypes_qtprotoreg/QtCore/QtCore_qtprotoreg.cpp.obj"
  )

list(APPEND _cmake_import_check_targets Qt6::ProtobufQtCoreTypes_qtprotoreg )
list(APPEND _cmake_import_check_files_for_Qt6::ProtobufQtCoreTypes_qtprotoreg "${_IMPORT_PREFIX}/lib/objects-Debug/ProtobufQtCoreTypes_qtprotoreg/QtCore/QtCore_qtprotoreg.cpp.obj" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
