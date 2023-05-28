project(${project_name}-lib C CXX)

file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.def

    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp

    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cc
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

file(GLOB_RECURSE public_headers
    ${CMAKE_CURRENT_LIST_DIR}/include/public/*.h
    ${CMAKE_CURRENT_LIST_DIR}/include/public/*.hpp
)

# =================================================================
if(HAS_EXPERIMENTAL_ITERATOR_H)
    set(HAS_EXPERIMENTAL_ITERATOR_H 1)
else()
    set(HAS_EXPERIMENTAL_ITERATOR_H 0)
endif()

# =================================================================
include(CMakeParseArguments)

function(add_lib_internal)
    set(options "")
    set(oneValueArgs TYPE ALIAS OUTPUT_LIB_NAME)
    set(multiValueArgs "")
    cmake_parse_arguments(ADD_LIB_INTERNAL "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

    set(type "${ADD_LIB_INTERNAL_TYPE}")
    set(alias "${ADD_LIB_INTERNAL_ALIAS}")
    set(output_lib_name "${ADD_LIB_INTERNAL_OUTPUT_LIB_NAME}")

    string(TOLOWER "${type}" suffix)
    set(internal_lib_name ${project_name}-${suffix}-lib)

    message(STATUS "Adding library for '${internal_lib_name}' with alias as '${alias}'")

    if("${type}" STREQUAL "STATIC")
        add_library(${internal_lib_name}
            STATIC ${src}
        )
    else()
        add_library(${internal_lib_name}
            SHARED ${src}
        )
    endif()

    if(output_lib_name)
        set_target_properties(${internal_lib_name} PROPERTIES OUTPUT_NAME ${output_lib_name})
        set_target_properties(${internal_lib_name} PROPERTIES PREFIX "")
    endif()

    if(NOT(alias STREQUAL ""))
        add_library(${alias} ALIAS ${internal_lib_name})
    endif()

    target_include_directories(${internal_lib_name}
        PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/public"
        PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include/private"
    )

    target_compile_definitions(${internal_lib_name}
        PUBLIC HAS_EXPERIMENTAL_ITERATOR_H=0
    )

    target_link_libraries(${internal_lib_name}
        PUBLIC llama
        PRIVATE common
        PUBLIC nlohmann_json::nlohmann_json
    )

    if(Boost_FOUND)
        target_link_libraries(${internal_lib_name}
            PUBLIC ${Boost_LIBRARIES}
        )

        target_compile_definitions(${internal_lib_name} PUBLIC "LLAMA_CPP_WRAPPER_USE_BOOST")
    endif()
endfunction()

# =================================================================
add_lib_internal(TYPE SHARED ALIAS llama_cpp::llama_cpp-shared OUTPUT_LIB_NAME libllama)

add_lib_internal(TYPE STATIC ALIAS llama_cpp::llama_cpp)

make_runtime_native_nupkg(llama-cpp-wrapper-shared-lib 
    "${LLAMA_CPP_WRAPPER_NUGET_PACKAGE_NAMESPACE}" 
    "runtime.native.llama.cpp.${CMAKE_CXX_COMPILER_ID}")
