# Additional target information for Qt6AxContainerTools
if(NOT DEFINED QT_DEFAULT_IMPORT_CONFIGURATION)
    set(QT_DEFAULT_IMPORT_CONFIGURATION RELWITHDEBINFO)
endif()
get_target_property(_qt_imported_location Qt6::dumpdoc IMPORTED_LOCATION_RELWITHDEBINFO)
get_target_property(_qt_imported_location_default Qt6::dumpdoc IMPORTED_LOCATION_${QT_DEFAULT_IMPORT_CONFIGURATION})

# Import target "Qt6::dumpdoc" for configuration "Release"
set_property(TARGET Qt6::dumpdoc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

if(_qt_imported_location)
    set_property(TARGET Qt6::dumpdoc PROPERTY IMPORTED_LOCATION_RELEASE "${_qt_imported_location}")
endif()

# Import target "Qt6::dumpdoc" for configuration "MinSizeRel"
set_property(TARGET Qt6::dumpdoc APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)

if(_qt_imported_location)
    set_property(TARGET Qt6::dumpdoc PROPERTY IMPORTED_LOCATION_MINSIZEREL "${_qt_imported_location}")
endif()

# Default configuration
if(_qt_imported_location_default)
    set_property(TARGET Qt6::dumpdoc PROPERTY IMPORTED_LOCATION "${_qt_imported_location_default}")
endif()
get_target_property(_qt_imported_location Qt6::dumpcpp IMPORTED_LOCATION_RELWITHDEBINFO)
get_target_property(_qt_imported_location_default Qt6::dumpcpp IMPORTED_LOCATION_${QT_DEFAULT_IMPORT_CONFIGURATION})

# Import target "Qt6::dumpcpp" for configuration "Release"
set_property(TARGET Qt6::dumpcpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

if(_qt_imported_location)
    set_property(TARGET Qt6::dumpcpp PROPERTY IMPORTED_LOCATION_RELEASE "${_qt_imported_location}")
endif()

# Import target "Qt6::dumpcpp" for configuration "MinSizeRel"
set_property(TARGET Qt6::dumpcpp APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)

if(_qt_imported_location)
    set_property(TARGET Qt6::dumpcpp PROPERTY IMPORTED_LOCATION_MINSIZEREL "${_qt_imported_location}")
endif()

# Default configuration
if(_qt_imported_location_default)
    set_property(TARGET Qt6::dumpcpp PROPERTY IMPORTED_LOCATION "${_qt_imported_location_default}")
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
