cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

if(POLICY CMP0058)
    cmake_policy(SET CMP0058 NEW)
endif()

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 OLD)
endif()

set(project_name llama-cpp-wrapper)

project(${project_name} C CXX)

option(LLAMA_CPP_WRAPPER_USE_OPENBLAS "Build with OpenBlas" OFF)
option(LLAMA_CPP_WRAPPER_USE_AVX512 "Build with AVX512" OFF)

set(LLAMA_CPP_WRAPPER_NUGET_PACKAGE_NAMESPACE "llama_cpp" CACHE STRING "LLAMA_CPP_WRAPPER_NUGET_PACKAGE_NAMESPACE" FORCE)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "Set to 11 or 14 to enable C++11 or C++17 builds, respectively." FORCE)
set(CMAKE_C_STANDARD 11 CACHE STRING "Set to 99 or 11 to enable C99 or C11 builds, respectively." FORCE)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(${CMAKE_BINARY_DIR})

include(FixCompileFlags)

if(NOT(CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_compile_definitions(OATPP_DISABLE_ENV_OBJECT_COUNTERS)
endif()

include(external/InitializeExternalComponents.cmake)

add_subdirectory(src)