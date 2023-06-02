project(${project_name}-assets C CXX)

execute_process(
    COMMAND npm install --save-dev
    COMMAND npm run build
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

file(GLOB_RECURSE assets
    ${CMAKE_CURRENT_LIST_DIR}/dist/*
)

cmrc_add_resource_library(${project_name}-assets NAMESPACE llama_cpp::server::rc ${assets})
