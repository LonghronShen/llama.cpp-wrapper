project(${project_name}-server-lib C CXX)

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

add_library(${project_name}-server-lib
    STATIC ${src}
)

if(NOT(CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_compile_definitions(OATPP_DISABLE_ENV_OBJECT_COUNTERS)
    target_compile_definitions(${project_name}-server-lib
        PUBLIC OATPP_DISABLE_ENV_OBJECT_COUNTERS
    )
endif()

target_link_libraries(${project_name}-server-lib
    PUBLIC oatpp
    PUBLIC oatpp-test
    PUBLIC oatpp-swagger
    PUBLIC llama_cpp::llama_cpp
    PUBLIC cmrc::base
    PRIVATE ${project_name}-assets
)

target_include_directories(${project_name}-server-lib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/public"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include/private"
)
