project(swagger-ui-assets C CXX)

file(GLOB_RECURSE assets
    ${CMAKE_CURRENT_LIST_DIR}/*.png
    ${CMAKE_CURRENT_LIST_DIR}/*.ico
    ${CMAKE_CURRENT_LIST_DIR}/*.html
    ${CMAKE_CURRENT_LIST_DIR}/*.js
    ${CMAKE_CURRENT_LIST_DIR}/*.css
    ${CMAKE_CURRENT_LIST_DIR}/*.map
)

cmrc_add_resource_library(swagger-ui-assets NAMESPACE swagger::ui::rc ${assets})
