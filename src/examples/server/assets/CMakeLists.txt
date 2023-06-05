project(${project_name}-assets C CXX)

find_program(NPM_EXECUTABLE NAMES npm REQUIRED)

if(WIN32)
    set(NPM_EXECUTABLE "${NPM_EXECUTABLE}.cmd")
endif()

message(STATUS "Found npm: ${NPM_EXECUTABLE}")

execute_process(
    COMMAND "${NPM_EXECUTABLE}" install --save-dev
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULTS_VARIABLE NPM_INSTALL_RESULT
    COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
    COMMAND "${NPM_EXECUTABLE}" run build
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULTS_VARIABLE NPM_BUILD_RESULT
    COMMAND_ERROR_IS_FATAL ANY
)

file(GLOB_RECURSE assets
    ${CMAKE_CURRENT_LIST_DIR}/dist/*
)

cmrc_add_resource_library(${project_name}-assets NAMESPACE llama_cpp::server::rc ${assets})
