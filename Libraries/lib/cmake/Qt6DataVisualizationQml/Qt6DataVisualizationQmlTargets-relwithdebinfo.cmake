#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::DataVisualizationQml" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::DataVisualizationQml APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::DataVisualizationQml PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6DataVisualizationQml.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "Qt6::Core;Qt6::Gui;Qt6::OpenGL;Qt6::Qml;Qt6::Quick;Qt6::DataVisualization"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6DataVisualizationQml.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::DataVisualizationQml )
list(APPEND _cmake_import_check_files_for_Qt6::DataVisualizationQml "${_IMPORT_PREFIX}/lib/Qt6DataVisualizationQml.lib" "${_IMPORT_PREFIX}/bin/Qt6DataVisualizationQml.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
