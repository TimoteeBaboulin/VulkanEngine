# Additional target information for Qt6ProtobufQtGuiTypes
if(NOT DEFINED QT_DEFAULT_IMPORT_CONFIGURATION)
    set(QT_DEFAULT_IMPORT_CONFIGURATION RELWITHDEBINFO)
endif()
__qt_internal_promote_target_to_global_checked(Qt6::ProtobufQtGuiTypes)
get_target_property(_qt_imported_location Qt6::ProtobufQtGuiTypes IMPORTED_LOCATION_RELWITHDEBINFO)
get_target_property(_qt_imported_implib Qt6::ProtobufQtGuiTypes IMPORTED_IMPLIB_RELWITHDEBINFO)
get_target_property(_qt_imported_link_dependencies Qt6::ProtobufQtGuiTypes IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO)
get_target_property(_qt_imported_location_default Qt6::ProtobufQtGuiTypes IMPORTED_LOCATION_${QT_DEFAULT_IMPORT_CONFIGURATION})
get_target_property(_qt_imported_implib_default Qt6::ProtobufQtGuiTypes IMPORTED_IMPLIB_${QT_DEFAULT_IMPORT_CONFIGURATION})
get_target_property(_qt_imported_link_dependencies_default Qt6::ProtobufQtGuiTypes IMPORTED_LINK_DEPENDENT_LIBRARIES_${QT_DEFAULT_IMPORT_CONFIGURATION})

# Import target "Qt6::ProtobufQtGuiTypes" for configuration "Release"
set_property(TARGET Qt6::ProtobufQtGuiTypes APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

if(_qt_imported_location)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_LOCATION_RELEASE "${_qt_imported_location}")
endif()
if(_qt_imported_implib)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_IMPLIB_RELEASE "${_qt_imported_implib}")
endif()
if(_qt_imported_link_dependencies)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "${_qt_imported_link_dependencies}")
endif()

# Import target "Qt6::ProtobufQtGuiTypes" for configuration "MinSizeRel"
set_property(TARGET Qt6::ProtobufQtGuiTypes APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)

if(_qt_imported_location)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_LOCATION_MINSIZEREL "${_qt_imported_location}")
endif()
if(_qt_imported_implib)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_IMPLIB_MINSIZEREL "${_qt_imported_implib}")
endif()
if(_qt_imported_link_dependencies)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_LINK_DEPENDENT_LIBRARIES_MINSIZEREL "${_qt_imported_link_dependencies}")
endif()

# Default configuration
if(_qt_imported_location_default)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_LOCATION "${_qt_imported_location_default}")
endif()
if(_qt_imported_implib_default)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_IMPLIB "${_qt_imported_implib_default}")
endif()
if(_qt_imported_link_dependencies_default)
    set_property(TARGET Qt6::ProtobufQtGuiTypes PROPERTY IMPORTED_LINK_DEPENDENT_LIBRARIES "${_qt_imported_link_dependencies_default}")
endif()
__qt_internal_promote_target_to_global_checked(Qt6::ProtobufQtGuiTypes_qtprotoreg)
get_target_property(_qt_imported_objects Qt6::ProtobufQtGuiTypes_qtprotoreg IMPORTED_OBJECTS_RELWITHDEBINFO)
get_target_property(_qt_imported_clr Qt6::ProtobufQtGuiTypes_qtprotoreg IMPORTED_COMMON_LANGUAGE_RUNTIME_RELWITHDEBINFO)
get_target_property(_qt_imported_objects_default Qt6::ProtobufQtGuiTypes_qtprotoreg IMPORTED_OBJECTS_${QT_DEFAULT_IMPORT_CONFIGURATION})
get_target_property(_qt_imported_clr_default Qt6::ProtobufQtGuiTypes_qtprotoreg IMPORTED_COMMON_LANGUAGE_RUNTIME_${QT_DEFAULT_IMPORT_CONFIGURATION})

# Import target "Qt6::ProtobufQtGuiTypes_qtprotoreg" for configuration "Release"
set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

if(_qt_imported_objects)
    set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg PROPERTY IMPORTED_OBJECTS_RELEASE "${_qt_imported_objects}")
endif()
if(_qt_imported_clr)
    set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg PROPERTY IMPORTED_COMMON_LANGUAGE_RUNTIME_RELEASE "${_qt_imported_clr}")
endif()

# Import target "Qt6::ProtobufQtGuiTypes_qtprotoreg" for configuration "MinSizeRel"
set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)

if(_qt_imported_objects)
    set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg PROPERTY IMPORTED_OBJECTS_MINSIZEREL "${_qt_imported_objects}")
endif()
if(_qt_imported_clr)
    set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg PROPERTY IMPORTED_COMMON_LANGUAGE_RUNTIME_MINSIZEREL "${_qt_imported_clr}")
endif()

# Default configuration
if(_qt_imported_objects_default)
    set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg PROPERTY IMPORTED_OBJECTS "${_qt_imported_objects_default}")
endif()
if(_qt_imported_clr_default)
    set_property(TARGET Qt6::ProtobufQtGuiTypes_qtprotoreg PROPERTY IMPORTED_COMMON_LANGUAGE_RUNTIME "${_qt_imported_clr_default}")
endif()

unset(_qt_imported_location)
unset(_qt_imported_location_default)
unset(_qt_imported_soname)
unset(_qt_imported_soname_default)
unset(_qt_imported_link_dependencies)
unset(_qt_imported_link_dependencies_default)
unset(_qt_imported_objects)
unset(_qt_imported_objects_default)
unset(_qt_imported_clr)
unset(_qt_imported_clr_default)
unset(_qt_imported_configs)
