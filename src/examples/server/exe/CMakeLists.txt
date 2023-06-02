project(${project_name}-server-exe C CXX)

file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(${project_name}-server-exe
    ${src}
)

target_link_libraries(${project_name}-server-exe
    PUBLIC ${project_name}-server-lib
)

if(MINGW)
    mingw_bundle_dll(${project_name}-server-exe)
endif()
