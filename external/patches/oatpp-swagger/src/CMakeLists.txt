file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.def

    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp

    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cc
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(${OATPP_THIS_MODULE_NAME} STATIC ${src})

set_target_properties(${OATPP_THIS_MODULE_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${OATPP_THIS_MODULE_NAME}
    PUBLIC oatpp
    PUBLIC oatpp-test
)

target_include_directories(${OATPP_THIS_MODULE_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}"
)

# # TODO link dependencies here (if some)

# ######################################################################################################
# # install targets
if(OATPP_INSTALL)
    include("../cmake/module-install.cmake")

    # # install swagger-ui files
    install(DIRECTORY ../res
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/oatpp-${OATPP_MODULE_VERSION}/bin/${OATPP_MODULE_NAME}"
        FILES_MATCHING PATTERN "*.*"
    )

    # # set environment variable to point to swagger-ui res files
    set(ENV{OATPP_SWAGGER_RES_PATH} "${CMAKE_INSTALL_INCLUDEDIR}/oatpp-${OATPP_MODULE_VERSION}/bin/${OATPP_MODULE_NAME}/res")
endif()
