cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME septum)
set(PROJECT_URL "https://github.com/ManifoldFR/pin-viz-cpp")
set(PROJECT_DESCRIPTION "A C++-based visualizer for Pinocchio using rerun")
set(PROJECT_CUSTOM_HEADER_EXTENSION "hpp")
set(PROJECT_VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_DISABLE_WERROR True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)
cmake_policy(SET CMP0135 OLD)

include(CMakeDependentOption)
include(cmake-module/base.cmake)
compute_project_args(PROJECT_ARGS LANGUAGES CXX)
project(${PROJECT_NAME} ${PROJECT_ARGS})

find_package(pinocchio REQUIRED)
find_package(assimp REQUIRED)
find_package(fmt 10.0 REQUIRED)

set(INSTALL_DOCUMENTATION OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH True)

FetchContent_Declare(
  rerun_sdk
  URL https://github.com/rerun-io/rerun/releases/download/0.14.1/rerun_cpp_sdk.zip)
FetchContent_GetProperties(rerun_sdk)
if(NOT rerun_sdk_POPULATED)
  # Fetch the content using previously declared details
  FetchContent_Populate(rerun_sdk)
  add_subdirectory(${rerun_sdk_SOURCE_DIR} ${rerun_sdk_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set_target_properties(rerun_sdk PROPERTIES POSITION_INDEPENDENT_CODE ON)

option(BUILD_PYTHON_INTERFACE "Build the Python bindings for this library." ON)
cmake_dependent_option(
  GENERATE_PYTHON_STUBS "Generate stub files for the Python library." ON
  BUILD_PYTHON_INTERFACE OFF)

set(LIB_NAME ${PROJECT_NAME})
file(GLOB lib_SRC CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB lib_HEADERS CONFIGURE_DEPENDS "src/*.hpp")

add_library(${LIB_NAME} SHARED "${lib_SRC}")

set(SEPTUM_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${SEPTUM_SRC_DIR}>
                                              $<INSTALL_INTERFACE:include>)
set(_build_dir_includes ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME})
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${_build_dir_includes}>)
target_link_libraries(${LIB_NAME} PUBLIC rerun_sdk)
target_link_libraries(${LIB_NAME} PRIVATE fmt::fmt)
target_link_libraries(${LIB_NAME} PUBLIC assimp::assimp)
target_link_libraries(${LIB_NAME} PUBLIC pinocchio::pinocchio)

install(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
install(FILES ${lib_HEADERS} DESTINATION include/${PROJECT_NAME})

if(BUILD_PYTHON_INTERFACE)
  add_subdirectory(python)
endif(BUILD_PYTHON_INTERFACE)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
