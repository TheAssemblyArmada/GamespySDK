cmake_minimum_required(VERSION 3.14)

# Use packagename_ROOT for FindPackage.
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

# Disable default MSVC warning level so we can set it ourselves.
if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW)
endif()

project(gamespy VERSION 2.06 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(FeatureSummary)

# Default to release for single target generators if nothing specified.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
endif()

# We don't support in tree builds, so help people make the right choice.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif()

# Decide if this project is the top level project, if so enable development targets.
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# Do we want OpenSSL support or integrated GameSpy SSLv3 engine support?
option(GS_OPENSSL "Enable OpenSSL support" ON)
option(GS_HTTPLOG "Enable HTTP logging" OFF)
option(GS_SECURESECONNECT "Enable connecting to GameSpy services with a secure protocol (HTTPS)" ON)
option(GS_BUILD_TESTS "Builds test executables." ${is_top_level})
option(GS_INCLUDE_VOICE "Build the Voice2 SDK component." OFF)

add_feature_info(OpenSSL GS_OPENSSL "Use OpenSSL library for HTTPS support.")
add_feature_info(HttpLogging GS_HTTPLOG "Log HTTP activity.")
add_feature_info(SecureConnections GS_SECURESECONNECT "Connect to services using HTTPS.")
add_feature_info(BuildTests GS_BUILD_TESTS "Build small test applications to demo the library.")
add_feature_info(IncludeVoiceSDK GS_INCLUDE_VOICE "Library will include the Voice2 SDK component.")

if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    list(APPEND GS_COMPILE_DEFS _CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_DEPRECATE _WINSOCK_DEPRECATED_NO_WARNINGS)
    
	# WinSock1 is deprecated, it's better to use WinSock2
    option(GS_WINSOCK2 "Link against Windows Socket 2 instead of Windows Socket 1." ON)
    add_feature_info(UseWinsock2 GS_WINSOCK2 "Library will link against winsock2 instead of winsock1.")
endif()

if(GAMESPY_SERVER_NAME)
    add_definitions(-DGSI_DOMAIN_NAME="${GAMESPY_SERVER_NAME}")
    message(STATUS "GameSpy server name is set to ${GAMESPY_SERVER_NAME}")
endif()

list(APPEND GS_COMPILE_DEFS GS_PEER)

if(GS_WINSOCK2)
    list(APPEND GS_COMPILE_DEFS GSI_WINSOCK2)
endif()

if(GS_SECURESECONNECT)
    list(APPEND GS_COMPILE_DEFS GS_HTTPS)
endif()

if(GS_MEM_MANAGED)
    list(APPEND GS_COMPILE_DEFS GSI_MEM_MANAGED)
endif()

if(is_top_level)
    # Set up a format target to do automated clang format checking.
    find_package(ClangFormat)
    include(ClangFormat)
endif()

if(DEFINED gamespy_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${gamespy_SHARED_LIBS}")
elseif(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
endif()

if(GS_INCLUDE_VOICE)
    # Speex is needed to build the voice2 SDK.
    find_package(Speex)

    if(NOT SPEEX_FOUND)
        include(FetchContent)

        FetchContent_Declare(
            speex_git
            GIT_REPOSITORY https://github.com/TheAssemblyArmada/speex.git
            GIT_TAG        1462ebd7f0b022567dafbd1b0e645994cff78577
        )

        # We don't use FetchContent_MakeAvailable here because we don't want all speex targets including, just our dependencies.
        FetchContent_GetProperties(speex_git)
        if(NOT speex_git_POPULATED)
            FetchContent_Populate(speex_git)    
            set(ENABLE_PACKAGE_CONFIG FALSE)
            add_subdirectory(${speex_git_SOURCE_DIR} ${speex_git_BINARY_DIR} EXCLUDE_FROM_ALL)
        endif()
        set(SPEEX_LIBRARIES speex)
    endif()
endif()

if(GS_OPENSSL)
    find_package(OpenSSL COMPONENTS SSL)

    if (NOT OPENSSL_FOUND)
        include(FetchContent)

        FetchContent_Declare(
            openssl_git
            GIT_REPOSITORY https://github.com/janbar/openssl-cmake.git
            GIT_TAG        e0224176e43dbbfb5308ae5f3d44611180138419
        )

        # We don't use FetchContent_MakeAvailable here because we don't want all OpenSSL targets including, just our dependencies.
        FetchContent_GetProperties(openssl_git)
        if(NOT openssl_git_POPULATED)
            FetchContent_Populate(openssl_git)
            set(ENABLE_PACKAGE_CONFIG FALSE)
            # TODO currently we don't support linking the static runtime.
            set(MSVC_DYNAMIC_RUNTIME TRUE)
            add_subdirectory(${openssl_git_SOURCE_DIR} ${openssl_git_BINARY_DIR} EXCLUDE_FROM_ALL)
        endif()
        add_library(OpenSSL::SSL ALIAS ssl)
    endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)
