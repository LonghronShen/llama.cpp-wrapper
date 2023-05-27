find_program(NUGET_EXECUTABLE NAMES nuget HINTS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

if(NOT NUGET_EXECUTABLE)
    message(STATUS "Check for nuget Program: not found")
    message(STATUS "Downloading nuget...")

    file(DOWNLOAD
        https://dist.nuget.org/win-x86-commandline/v6.5.0/nuget.exe
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nuget.exe
        EXPECTED_HASH SHA1=ffd53a0bf4353752522217e64009c441ac219f63
        SHOW_PROGRESS
    )

    set(NUGET_EXECUTABLE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nuget.exe")

    if(NOT WIN32)
        find_program(MONO_EXECUTABLE NAMES mono REQUIRED)
    endif()
else()
    message(STATUS "Found nuget Program: ${NUGET_EXECUTABLE}")
endif()

# Find dotnet cli
find_program(DOTNET_EXECUTABLE NAMES dotnet)

if(DOTNET_EXECUTABLE)
    message(STATUS "Found dotnet Program: ${DOTNET_EXECUTABLE}")
endif()

set(DOTNET_PACKAGES_DIR "${PROJECT_BINARY_DIR}/dotnet/packages")

# see: https://docs.microsoft.com/en-us/dotnet/core/rid-catalog
if(APPLE)
    exec_program(/usr/bin/sw_vers ARGS -productVersion OUTPUT_VARIABLE DARWIN_VERSION)

    set(RUNTIME_PLATFORM osx)
    set(RUNTIME_PLATFORM_VERSION "${DARWIN_VERSION}")

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)")
        set(RUNTIME_IDENTIFIER osx-arm64)
        set(RUNTIME_PLATFORM_ARCH "arm64")
    else()
        set(RUNTIME_IDENTIFIER osx-x64)
        set(RUNTIME_PLATFORM_ARCH "x64")
    endif()
elseif(UNIX)
    set(RUNTIME_PLATFORM linux)

    find_program(LSB_RELEASE_EXEC lsb_release)

    if(NOT LSB_RELEASE_EXEC)
        set(RUNTIME_PLATFORM_VERSION "any")
    else()
        execute_process(COMMAND bash -c "${LSB_RELEASE_EXEC} -is | tr \"[:upper:]\" \"[:lower:]\""
            OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        execute_process(COMMAND bash -c "${LSB_RELEASE_EXEC} -rs | tr \"[:upper:]\" \"[:lower:]\""
            OUTPUT_VARIABLE LSB_RELEASE_VER_SHORT
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        set(RUNTIME_PLATFORM "${LSB_RELEASE_ID_SHORT}")
        set(RUNTIME_PLATFORM_VERSION "${LSB_RELEASE_VER_SHORT}")
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)")
        set(RUNTIME_IDENTIFIER linux-arm64)
        set(RUNTIME_PLATFORM_ARCH "arm64")
    else()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(RUNTIME_IDENTIFIER linux-x64)
            set(RUNTIME_PLATFORM_ARCH "x64")
        else()
            set(RUNTIME_IDENTIFIER linux-x86)
            set(RUNTIME_PLATFORM_ARCH "x86")
        endif()
    endif()
elseif(WIN32)
    set(RUNTIME_PLATFORM win)
    set(RUNTIME_PLATFORM_VERSION "any")

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^ARM64")
        set(RUNTIME_IDENTIFIER win-arm64)
        set(RUNTIME_PLATFORM_ARCH "arm64")
    else()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(RUNTIME_IDENTIFIER win-x64)
            set(RUNTIME_PLATFORM_ARCH "x64")
        else()
            set(RUNTIME_IDENTIFIER win-x86)
            set(RUNTIME_PLATFORM_ARCH "x86")
        endif()
    endif()
else()
    message(FATAL_ERROR "Unsupported system!")
endif()

if(WIN32)
    set(RUNTIME_NUGET_PLATFORM_IDENTIFIER "${RUNTIME_PLATFORM}-${RUNTIME_PLATFORM_ARCH}")
else()
    set(RUNTIME_NUGET_PLATFORM_IDENTIFIER "${RUNTIME_PLATFORM}.${RUNTIME_PLATFORM_VERSION}-${RUNTIME_PLATFORM_ARCH}")
endif()

message(STATUS "Runtime NuGet Package platform identifier: ${RUNTIME_NUGET_PLATFORM_IDENTIFIER}")

function(make_runtime_native_nupkg target namespace id)
    set(spec_template "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/runtime.native.nuspec.in")

    get_target_property(_LIB_OUTPUT_NAME ${target} OUTPUT_NAME)
    get_target_property(_LIB_VERSION ${target} VERSION)

    set(_LIB_LOCATION $<TARGET_FILE:${target}>)

    if(NOT _LIB_VERSION)
        set(_LIB_VERSION "1.0.0")

        if(DEFINED ENV{RELEASE_VERSION})
            set(_LIB_VERSION "$ENV{RELEASE_VERSION}")
        endif()
    endif()

    set(RUNTIME_NUGET_PACKAGE_FULL_ID "${namespace}.runtime.${RUNTIME_NUGET_PLATFORM_IDENTIFIER}.${id}")
    set(RUNTIME_NUGET_PACKAGE_FULL_TITLE "${RUNTIME_NUGET_PACKAGE_FULL_ID}")
    set(RUNTIME_NUGET_PACKAGE_VERSION "${_LIB_VERSION}")
    set(RUNTIME_NUGET_PLATFORM_NATIVE_FILE_SRC "${_LIB_LOCATION}")
    set(RUNTIME_NUGET_PACKAGE_PROJECT_DESCRIPTION "Native pacakge for ${RUNTIME_NUGET_PACKAGE_FULL_ID}")
    set(RUNTIME_NUGET_PACKAGE_PROJECT_LICENSE_URL "")

    execute_process(COMMAND bash -c "git log --format='%an' | tr \"[:upper:]\" \"[:lower:]\" | sort | uniq | paste -s -d, -"
        OUTPUT_VARIABLE RUNTIME_NUGET_PACKAGE_AUTHORS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(COMMAND git remote get-url origin
        OUTPUT_VARIABLE RUNTIME_NUGET_PACKAGE_PROJECT_URL
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(target_file_prefix "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RUNTIME_NUGET_PACKAGE_FULL_ID}")
    set(target_spec_file "${target_file_prefix}.nuspec")
    set(target_nupkg_file "${target_file_prefix}.nupkg")

    configure_file(
        "${spec_template}"
        "${target_spec_file}"
        @ONLY)

    if(MONO_EXECUTABLE)
        add_custom_command(
            OUTPUT "${target_nupkg_file}"
            COMMAND ${MONO_EXECUTABLE} ${NUGET_EXECUTABLE} pack "${target_spec_file}" -BasePath $<TARGET_FILE_DIR:${target}>
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            DEPENDS "${target}"
            COMMENT "Building NuGet package for ${RUNTIME_NUGET_PACKAGE_FULL_ID} in '$<TARGET_FILE_DIR:${target}>'"
            VERBATIM
        )
    else()
        add_custom_command(
            OUTPUT "${target_nupkg_file}"
            COMMAND ${NUGET_EXECUTABLE} pack "${target_spec_file}" -BasePath $<TARGET_FILE_DIR:${target}>
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            DEPENDS "${target}"
            COMMENT "Building NuGet package for ${RUNTIME_NUGET_PACKAGE_FULL_ID} in '$<TARGET_PROPERTY:${target},LIBRARY_OUTPUT_DIRECTORY>/${BIN_DIR}'"
            VERBATIM
        )
    endif()

    add_custom_target("nupkg_${target}" ALL DEPENDS "${target_nupkg_file}")

    # RUNTIME_NUGET_PACKAGE_FULL_ID
    # RUNTIME_NUGET_PACKAGE_VERSION
    # RUNTIME_NUGET_PACKAGE_FULL_TITLE
    # RUNTIME_NUGET_PACKAGE_AUTHORS
    # RUNTIME_NUGET_PACKAGE_PROJECT_LICENSE_URL
    # RUNTIME_NUGET_PACKAGE_PROJECT_URL
    # RUNTIME_NUGET_PACKAGE_PROJECT_ICON_URL
    # RUNTIME_NUGET_PACKAGE_PROJECT_DESCRIPTION
endfunction()