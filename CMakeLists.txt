cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME septum)
set(PROJECT_URL "")
set(PROJECT_DESCRIPTION "")
set(PROJECT_CUSTOM_HEADER_EXTENSION "hpp")

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

set(BUILD_SHARED_LIBS ON)
set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH True)

FetchContent_Declare(
  rerun_sdk
  URL https://github.com/rerun-io/rerun/releases/latest/download/rerun_cpp_sdk.zip)
FetchContent_MakeAvailable(rerun_sdk)

option(BUILD_PYTHON_INTERFACE "Build the Python bindings for this library." ON)
cmake_dependent_option(
  GENERATE_PYTHON_STUBS "Generate stub files for the Python library." ON
  BUILD_PYTHON_INTERFACE OFF)

set(LIB_NAME ${PROJECT_NAME})
file(GLOB lib_SRC CONFIGURE_DEPENDS "src/*.cpp")

message(STATUS "Library sources: ${lib_SRC}")
add_library(${LIB_NAME} SHARED "${lib_SRC}")

target_include_directories(${LIB_NAME} PUBLIC src/)
target_link_libraries(${LIB_NAME} PUBLIC rerun_sdk)
target_link_libraries(${LIB_NAME} PRIVATE fmt::fmt)
target_link_libraries(${LIB_NAME} PUBLIC assimp::assimp)
target_link_libraries(${LIB_NAME} PUBLIC pinocchio::pinocchio)

set_target_properties(${LIB_NAME} PROPERTIES INSTALL_RPATH "\$ORIGIN/${PROJECT_NAME}")
install(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

set_target_properties(rerun_sdk PROPERTIES INSTALL_RPATH "\$ORIGIN/../../lib")
set(RERUN_INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/${PROJECT_NAME})
install(TARGETS rerun_sdk LIBRARY DESTINATION ${RERUN_INSTALL_DIR})

if(BUILD_PYTHON_INTERFACE)
  add_subdirectory(python)
endif(BUILD_PYTHON_INTERFACE)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
