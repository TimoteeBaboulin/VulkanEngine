#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Quick3DEffects" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::Quick3DEffects APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::Quick3DEffects PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/Qt6Quick3DEffects.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "Qt6::Core"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/Qt6Quick3DEffects.dll"
  )

list(APPEND _cmake_import_check_targets Qt6::Quick3DEffects )
list(APPEND _cmake_import_check_files_for_Qt6::Quick3DEffects "${_IMPORT_PREFIX}/lib/Qt6Quick3DEffects.lib" "${_IMPORT_PREFIX}/bin/Qt6Quick3DEffects.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
