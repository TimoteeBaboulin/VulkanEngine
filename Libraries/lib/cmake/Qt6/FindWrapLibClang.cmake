# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(TARGET WrapLibClang::WrapLibClang)
    set(WrapLibClang_FOUND TRUE)
    return()
endif()

if(DEFINED ENV{LLVM_INSTALL_DIR})
    set(__qt_wrap_clang_backup_prefix "${CMAKE_PREFIX_PATH}")
    list(PREPEND CMAKE_PREFIX_PATH "$ENV{LLVM_INSTALL_DIR}")
elseif(DEFINED CACHE{LLVM_INSTALL_DIR})
    set(__qt_wrap_clang_backup_prefix "${CMAKE_PREFIX_PATH}")
    list(PREPEND CMAKE_PREFIX_PATH "${LLVM_INSTALL_DIR}")
endif()

include(FindPackageHandleStandardArgs)
set(WrapLibClang_FOUND FALSE)

if(QT_NO_FIND_PACKAGE_CLANG_WORKAROUND)
    set(CLANG_FOUND FALSE)
    foreach(VERSION ${QDOC_SUPPORTED_CLANG_VERSIONS})
        if(NOT CLANG_FOUND)
            find_package(Clang ${VERSION} CONFIG QUIET)
        endif()
    endforeach()
else()
    set(__qt_wraplibclang_message
        "This probably means that one or more packages necessary for find_package(Clang) are not"
        "installed. See below for more information. You can turn off this pre-check by setting the"
        "CMake variable QT_NO_FIND_PACKAGE_CLANG_WORKAROUND to ON."
    )

    # Try to find the LLVM package. ClangConfig.cmake has a find_package(LLVM REQUIRED) call, which
    # will break if clang is installed but the LLVM CMake files are not installed.
    set(LLVM_FOUND FALSE)
    foreach(VERSION ${QDOC_SUPPORTED_CLANG_VERSIONS})
        if(NOT LLVM_FOUND)
            find_package(LLVM ${VERSION} CONFIG QUIET)
        endif()
    endforeach()
    if(NOT LLVM_FOUND)
        list(PREPEND __qt_wraplibclang_message "The LLVM package could not be found.")
        string(REPLACE ";" " " __qt_wraplibclang_message "${__qt_wraplibclang_message}")
        find_package_handle_standard_args(WrapLibClang
            REQUIRED_VARS WrapLibClang_FOUND
            REASON_FAILURE_MESSAGE "${__qt_wraplibclang_message}")
        unset(__qt_wraplibclang_message)
        return()
    endif()

    # Try to find libClang libraries - either one of the static libs or the whole shared object.
    # ClangTargets.cmake checks for the presence of these libraries.
    find_library(__qt_wraplibclang clangBasic HINTS ${LLVM_LIBRARY_DIRS})
    if(__qt_wraplibclang STREQUAL "__qt_wraplibclang-NOTFOUND")
        unset(__qt_wraplibclang CACHE)
        find_library(__qt_wraplibclang clang HINTS ${LLVM_LIBRARY_DIRS})
    endif()
    if(__qt_wraplibclang STREQUAL "__qt_wraplibclang-NOTFOUND")
        unset(__qt_wraplibclang CACHE)
        list(PREPEND __qt_wraplibclang_message "The clang libraries could not be located.")
        string(REPLACE ";" " " __qt_wraplibclang_message "${__qt_wraplibclang_message}")
        find_package_handle_standard_args(WrapLibClang
            REQUIRED_VARS WrapLibClang_FOUND
            REASON_FAILURE_MESSAGE "${__qt_wraplibclang_message}")
        unset(__qt_wraplibclang_message)
        return()
    endif()
    unset(__qt_wraplibclang CACHE)

    # Now, we're pretty certain that we can find the 'Clang' package without running into errors.
    find_package(Clang ${LLVM_VERSION} EXACT CONFIG)
endif()

# LLVM versions >= 16 come with Findzstd.cmake that creates a target for libzstd.
# Disable its global promotion to prevent interference with FindWrapZSTD.cmake.
if(TARGET zstd::libzstd)
    qt_internal_disable_find_package_global_promotion(zstd::libzstd)
endif()
if(TARGET zstd::libzstd_shared)
    qt_internal_disable_find_package_global_promotion(zstd::libzstd_shared)
endif()
if(TARGET zstd::libzstd_static)
    qt_internal_disable_find_package_global_promotion(zstd::libzstd_static)
endif()

if(__qt_wrap_clang_backup_prefix)
    set(CMAKE_PREFIX_PATH "${__qt_wrap_clang_backup_prefix}")
    unset(__qt_wrap_clang_backup_prefix)
endif()

set(__wrap_lib_clang_requested_version_found FALSE)

# Need to explicitly handle the version check, because the Clang package doesn't.
if(WrapLibClang_FIND_VERSION AND LLVM_PACKAGE_VERSION
        AND LLVM_PACKAGE_VERSION VERSION_GREATER_EQUAL "${WrapLibClang_FIND_VERSION}")
    set(__wrap_lib_clang_requested_version_found TRUE)
endif()

if(TARGET libclang AND ((TARGET clang-cpp AND TARGET LLVM) OR TARGET clangHandleCXX) AND __wrap_lib_clang_requested_version_found)
    set(WrapLibClang_FOUND TRUE)

    get_target_property(type libclang TYPE)
    if (MSVC AND type STREQUAL "STATIC_LIBRARY")
        get_property(__wrap_lib_clang_multi_config
            GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
        if(__wrap_lib_clang_multi_config)
            set(__wrap_lib_clang_configs ${CMAKE_CONFIGURATION_TYPES})
        else()
            set(__wrap_lib_clang_configs ${CMAKE_BUILD_TYPE})
        endif()
        set(__wrap_lib_clang_non_release_configs ${configs})
        list(REMOVE_ITEM __wrap_lib_clang_non_release_configs
            Release MinSizeRel RelWithDebInfo)
        if(__wrap_lib_clang_non_release_configs STREQUAL __wrap_lib_clang_configs)
            message(STATUS "Static linkage against libclang with MSVC was requested, but the build is not a release build, therefore libclang cannot be used.")
            set(WrapLibClang_FOUND FALSE)
        endif()
    endif()

    if(WrapLibClang_FOUND)
        add_library(WrapLibClang::WrapLibClang IMPORTED INTERFACE)

        target_include_directories(WrapLibClang::WrapLibClang INTERFACE ${CLANG_INCLUDE_DIRS})
        if (NOT TARGET Threads::Threads)
            find_package(Threads)
        endif()
        qt_internal_disable_find_package_global_promotion(Threads::Threads)
        # lupdate must also link to LLVM when using clang-cpp
        set(__qt_clang_genex_condition "$<AND:$<TARGET_EXISTS:clang-cpp>,$<TARGET_EXISTS:LLVM>>")
        set(__qt_clang_genex "$<IF:${__qt_clang_genex_condition},clang-cpp;LLVM,clangHandleCXX>")
        target_link_libraries(WrapLibClang::WrapLibClang
            INTERFACE libclang
            "${__qt_clang_genex}"
            Threads::Threads
            )

        foreach(version MAJOR MINOR PATCH)
            set(QT_LIB_CLANG_VERSION_${version} ${LLVM_VERSION_${version}} CACHE STRING "" FORCE)
        endforeach()
        set(QT_LIB_CLANG_VERSION ${LLVM_PACKAGE_VERSION} CACHE STRING "" FORCE)
        set(QT_LIB_CLANG_LIBDIR "${LLVM_LIBRARY_DIRS}" CACHE STRING "" FORCE)
        set(QT_LIBCLANG_RESOURCE_DIR
            "\"${QT_LIB_CLANG_LIBDIR}/clang/${QT_LIB_CLANG_VERSION}/include\"" CACHE STRING "" FORCE)
    endif()
endif()

find_package_handle_standard_args(WrapLibClang
    REQUIRED_VARS WrapLibClang_FOUND
    VERSION_VAR LLVM_PACKAGE_VERSION)
