find_package(Threads REQUIRED)

if(UNIX)
  find_package(DL REQUIRED)
endif()

set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE STRING "FETCHCONTENT_UPDATES_DISCONNECTED" FORCE)

include(FetchContent)

# boost-cmake
if("${PLATFORM_ID}" STREQUAL "linux-arm")
  set(Boost_USE_STATIC_LIBS OFF CACHE STRING "Boost_USE_STATIC_LIBS" FORCE)
else()
  set(Boost_USE_STATIC_LIBS ON CACHE STRING "Boost_USE_STATIC_LIBS" FORCE)
endif()

if(WIN32)
  set(Boost_USE_STATIC_RUNTIME ON CACHE STRING "Boost_USE_STATIC_RUNTIME" FORCE)

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(Boost_USE_DEBUG_LIBS ON CACHE STRING "Boost_USE_DEBUG_LIBS" FORCE)
    set(Boost_USE_DEBUG_RUNTIME ON CACHE STRING "Boost_USE_DEBUG_RUNTIME" FORCE)
  else()
    set(Boost_USE_DEBUG_LIBS OFF CACHE STRING "Boost_USE_DEBUG_LIBS" FORCE)
    set(Boost_USE_DEBUG_RUNTIME OFF CACHE STRING "Boost_USE_DEBUG_RUNTIME" FORCE)
  endif()
endif()

find_package(Boost 1.63 COMPONENTS thread log log_setup system program_options filesystem coroutine locale regex unit_test_framework serialization)

if(Boost_FOUND)
  message(STATUS "** Boost Include: ${Boost_INCLUDE_DIR}")
  message(STATUS "** Boost Libraries Directory: ${Boost_LIBRARY_DIRS}")
  message(STATUS "** Boost Libraries: ${Boost_LIBRARIES}")
else()
  if(WIN32)
    message(WARNING "Plase check your vcpkg settings or global environment variables for the boost library.")
  else()
    FetchContent_Declare(boost_cmake
      GIT_REPOSITORY https://github.com/Orphis/boost-cmake.git
      GIT_TAG master)

    FetchContent_GetProperties(boost_cmake)

    if(NOT boost_cmake_POPULATED)
      FetchContent_Populate(boost_cmake)
      add_subdirectory(${boost_cmake_SOURCE_DIR} ${boost_cmake_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
  endif()
endif()

# cmrc
FetchContent_Declare(cmrc
  GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
  GIT_TAG a64bea50c05594c8e7cf1f08e441bb9507742e2e)

FetchContent_GetProperties(cmrc)

if(NOT cmrc_POPULATED)
  FetchContent_Populate(cmrc)
  add_subdirectory(${cmrc_SOURCE_DIR} ${cmrc_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# nlohmann_json
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.10.4)

FetchContent_GetProperties(json)

if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# argh
FetchContent_Declare(argh
  GIT_REPOSITORY https://github.com/adishavit/argh.git
  GIT_TAG master)

FetchContent_GetProperties(argh)

if(NOT argh_POPULATED)
  FetchContent_Populate(argh)
  add_subdirectory(${argh_SOURCE_DIR} ${argh_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# parallel-hashmap
FetchContent_Declare(parallel_hashmap
  GIT_REPOSITORY https://github.com/greg7mdp/parallel-hashmap.git
  GIT_TAG master)

FetchContent_GetProperties(parallel_hashmap)

if(NOT parallel_hashmap_POPULATED)
  FetchContent_Populate(parallel_hashmap)
  add_subdirectory(${parallel_hashmap_SOURCE_DIR} ${parallel_hashmap_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# MimeTypes
FetchContent_Declare(mimetypes
  GIT_REPOSITORY https://github.com/LonghronShen/MimeTypes.git
  GIT_TAG master)

FetchContent_GetProperties(mimetypes)

if(NOT mimetypes_POPULATED)
  FetchContent_Populate(mimetypes)
  add_subdirectory(${mimetypes_SOURCE_DIR} ${mimetypes_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# isocline
FetchContent_Declare(isocline
  GIT_REPOSITORY https://github.com/daanx/isocline.git
  GIT_TAG main)

FetchContent_GetProperties(isocline)

if(NOT isocline_POPULATED)
  FetchContent_Populate(isocline)
  add_subdirectory(${isocline_SOURCE_DIR} ${isocline_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if(LLAMA_CPP_WRAPPER_USE_OPENBLAS)
  # OpenBLAS
  find_package(BLAS)

  if(NOT BLAS_FOUND)
    set(BUILD_STATIC_LIBS ON CACHE STRING "BUILD_STATIC_LIBS" FORCE)

    if(UNIX AND(NOT APPLE))
      set(DYNAMIC_ARCH ON CACHE STRING "DYNAMIC_ARCH" FORCE)
    endif()

    FetchContent_Declare(openblas
      GIT_REPOSITORY https://github.com/xianyi/OpenBLAS.git
      GIT_SHALLOW TRUE
      GIT_TAG develop)

    FetchContent_GetProperties(openblas)

    if(NOT openblas_POPULATED)
      FetchContent_Populate(openblas)
      add_subdirectory(${openblas_SOURCE_DIR} ${openblas_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
  endif()
endif()

# llama_cpp
set(LLAMA_BUILD_EXAMPLES ON CACHE STRING "LLAMA_BUILD_EXAMPLES" FORCE)

if(LLAMA_CPP_WRAPPER_USE_AVX512)
  set(LLAMA_AVX512 ON CACHE STRING "LLAMA_AVX512" FORCE)
  set(LLAMA_AVX512_VBMI ON CACHE STRING "LLAMA_AVX512_VBMI" FORCE)
  set(LLAMA_AVX512_VNNI ON CACHE STRING "LLAMA_AVX512_VNNI" FORCE)
endif()

FetchContent_Declare(llama_cpp
  GIT_REPOSITORY https://github.com/ggerganov/llama.cpp.git
  GIT_TAG master)

FetchContent_GetProperties(llama_cpp)

if(NOT llama_cpp_POPULATED)
  FetchContent_Populate(llama_cpp)
  add_subdirectory(${llama_cpp_SOURCE_DIR} ${llama_cpp_BINARY_DIR} EXCLUDE_FROM_ALL)

  set_target_properties(ggml PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
  )

  set_target_properties(llama PROPERTIES
    OUTPUT_NAME llama-cpp
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
  )

  if(LLAMA_CPP_WRAPPER_USE_OPENBLAS)
    target_compile_definitions(ggml PUBLIC "GGML_USE_OPENBLAS")

    if(BLAS_FOUND)
      target_link_libraries(ggml PUBLIC BLAS::BLAS)
    else()
      target_link_libraries(ggml PUBLIC openblas_static)
      target_include_directories(ggml
        PRIVATE ${openblas_BINARY_DIR}
        PRIVATE ${openblas_SOURCE_DIR})
    endif()
  endif()
endif()

# oatpp
set(OATPP_INSTALL OFF CACHE STRING "OATPP_INSTALL" FORCE)
set(OATPP_BUILD_TESTS OFF CACHE STRING "OATPP_BUILD_TESTS" FORCE)

if(MSVC)
  set(OATPP_MSVC_LINK_STATIC_RUNTIME OFF CACHE STRING "OATPP_MSVC_LINK_STATIC_RUNTIME" FORCE)
endif()

FetchContent_Declare(oatpp
  GIT_REPOSITORY https://github.com/oatpp/oatpp.git
  GIT_TAG master)

FetchContent_GetProperties(oatpp)

if(NOT oatpp_POPULATED)
  FetchContent_Populate(oatpp)
  add_subdirectory(${oatpp_SOURCE_DIR} ${oatpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# oatpp-swagger
set(OATPP_MODULES_LOCATION "CUSTOM" CACHE STRING "OATPP_MODULES_LOCATION" FORCE)

FetchContent_Declare(oatpp_swagger
  GIT_REPOSITORY https://github.com/oatpp/oatpp-swagger.git
  GIT_TAG master)

FetchContent_GetProperties(oatpp_swagger)

if(NOT oatpp_swagger_POPULATED)
  FetchContent_Populate(oatpp_swagger)
  file(COPY "${CMAKE_CURRENT_LIST_DIR}/patches/oatpp-swagger/src" DESTINATION "${oatpp_swagger_SOURCE_DIR}/")
  file(COPY "${CMAKE_CURRENT_LIST_DIR}/patches/oatpp-swagger/res" DESTINATION "${oatpp_swagger_SOURCE_DIR}/")
  add_subdirectory(${oatpp_swagger_SOURCE_DIR} ${oatpp_swagger_BINARY_DIR} EXCLUDE_FROM_ALL)
  add_subdirectory(${oatpp_swagger_SOURCE_DIR}/res ${oatpp_swagger_BINARY_DIR}/res EXCLUDE_FROM_ALL)
endif()

if(MINGW)
  # mingw-bundledlls
  FetchContent_Declare(mingw_bundledlls
    GIT_REPOSITORY https://github.com/LonghronShen/mingw-bundledlls.git
    GIT_TAG master)

  FetchContent_GetProperties(mingw_bundledlls)

  if(NOT mingw_bundledlls_POPULATED)
    FetchContent_Populate(mingw_bundledlls)

    find_package(Python3 REQUIRED)

    get_filename_component(MINGW_COMPILER_HOME ${CMAKE_CXX_COMPILER} DIRECTORY)

    file(REAL_PATH "${MINGW_COMPILER_HOME}/../" TOOLCHAIN_DIR)
    file(REAL_PATH "${MINGW_COMPILER_HOME}/../../" MSYS_DIR)

    set(MINGW_BUNDLEDLLS_SEARCH_PATH "${TOOLCHAIN_DIR}|${MSYS_DIR}")
    message(STATUS "Searching MinGW DLLs in: \"${MINGW_BUNDLEDLLS_SEARCH_PATH}\"")

    function(mingw_bundle_dll target_name)
      add_custom_target(${target_name}-deps ALL
        COMMAND ${CMAKE_COMMAND} -E env MINGW_BUNDLEDLLS_SEARCH_PATH=${MINGW_BUNDLEDLLS_SEARCH_PATH} -- "${Python3_EXECUTABLE}" "${mingw_bundledlls_SOURCE_DIR}/mingw-bundledlls" -l "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dependencies.log" --force --copy "$<TARGET_FILE:${target_name}>"
        WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
        DEPENDS ${target_name}
        COMMENT "Copying MinGW libs ..."
        VERBATIM
      )
    endfunction()
  endif()
endif()