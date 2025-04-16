# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was QtModuleConfigPrivate.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

cmake_minimum_required(VERSION 3.16...3.21)

include(CMakeFindDependencyMacro)

# Find required dependencies, if any.
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/Qt6OpenGLWidgetsPrivateDependencies.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/Qt6OpenGLWidgetsPrivateDependencies.cmake")
    _qt_internal_suggest_dependency_debugging(OpenGLWidgetsPrivate
        __qt_OpenGLWidgetsPrivate_pkg ${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE)
endif()

# If *ConfigDependencies.cmake exists, the variable value will be defined there.
# Don't override it in that case.
if(NOT DEFINED "Qt6OpenGLWidgetsPrivate_FOUND")
    set("Qt6OpenGLWidgetsPrivate_FOUND" TRUE)
endif()

if(NOT DEFINED QT_REPO_MODULE_VERSION
        AND NOT QT_NO_PRIVATE_MODULE_WARNING
        AND NOT __qt_private_module_OpenGLWidgetsPrivate_warning_shown)
    message(WARNING
        "This project is using headers of the OpenGLWidgetsPrivate module and will therefore be tied "
        "to this specific Qt module build version. "
        "Running this project against other versions of the Qt modules may crash at any arbitrary "
        "point. This is not a bug, but a result of using Qt internals. You have been warned! "
        "\nYou can disable this warning by setting QT_NO_PRIVATE_MODULE_WARNING to ON."
    )
    set(__qt_private_module_OpenGLWidgetsPrivate_warning_shown TRUE)
endif()

if(NOT QT_NO_CREATE_TARGETS AND Qt6OpenGLWidgetsPrivate_FOUND)
    include("${CMAKE_CURRENT_LIST_DIR}/Qt6OpenGLWidgetsPrivateTargets.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/Qt6OpenGLWidgetsPrivateAdditionalTargetInfo.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/Qt6OpenGLWidgetsPrivateExtraProperties.cmake"
        OPTIONAL)
endif()

if(TARGET Qt6::OpenGLWidgetsPrivate)
    if(NOT QT_NO_CREATE_VERSIONLESS_TARGETS)
        if(CMAKE_VERSION VERSION_LESS 3.18 OR QT_USE_OLD_VERSION_LESS_TARGETS)
            include("${CMAKE_CURRENT_LIST_DIR}/Qt6OpenGLWidgetsPrivateVersionlessTargets.cmake")
        else()
            include("${CMAKE_CURRENT_LIST_DIR}/Qt6OpenGLWidgetsPrivateVersionlessAliasTargets.cmake")
        endif()
    endif()
else()
    set(Qt6OpenGLWidgetsPrivate_FOUND FALSE)
    if(NOT DEFINED Qt6OpenGLWidgetsPrivate_NOT_FOUND_MESSAGE)
        set(Qt6OpenGLWidgetsPrivate_NOT_FOUND_MESSAGE
            "Target \"Qt6::OpenGLWidgetsPrivate\" was not found.")

        if(QT_NO_CREATE_TARGETS)
            string(APPEND Qt6OpenGLWidgetsPrivate_NOT_FOUND_MESSAGE
                "Possibly due to QT_NO_CREATE_TARGETS being set to TRUE and thus "
                "${CMAKE_CURRENT_LIST_DIR}/Qt6OpenGLWidgetsPrivateTargets.cmake was not "
                "included to define the target.")
        endif()
    endif()
endif()
