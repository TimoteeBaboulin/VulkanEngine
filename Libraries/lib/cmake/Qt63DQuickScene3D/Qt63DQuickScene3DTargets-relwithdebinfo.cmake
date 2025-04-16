#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::3DQuickScene3D" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::3DQuickScene3D APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::3DQuickScene3D PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt63DQuickScene3D.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "Qt6::3DRender;Qt6::Gui;Qt6::Qml;Qt6::3DInput;Qt6::3DLogic;Qt6::3DAnimation"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt63DQuickScene3D.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::3DQuickScene3D )
list(APPEND _cmake_import_check_files_for_Qt6::3DQuickScene3D "${_IMPORT_PREFIX}/lib/Qt63DQuickScene3D.lib" "${_IMPORT_PREFIX}/bin/Qt63DQuickScene3D.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
