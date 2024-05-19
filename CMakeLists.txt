cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME pinocchio_rerun)
set(PROJECT_URL "https://github.com/ManifoldFR/pinocchio-rerun")
set(PROJECT_DESCRIPTION "A C++-based visualizer for Pinocchio using rerun")
set(PROJECT_CUSTOM_HEADER_EXTENSION "hpp")
set(PROJECT_CUSTOM_HEADER_DIR ${PROJECT_NAME})
set(PROJECT_VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_DISABLE_WERROR True)

cmake_policy(SET CMP0135 OLD)

include(CMakeDependentOption)
include(cmake-module/base.cmake)
include(cmake-module/ide.cmake)
compute_project_args(PROJECT_ARGS LANGUAGES CXX)
project(${PROJECT_NAME} ${PROJECT_ARGS})

add_project_dependency(pinocchio REQUIRED)
add_project_dependency(pinocchio_visualizers REQUIRED)
add_project_dependency(assimp REQUIRED)
add_project_dependency(fmt 10.0 REQUIRED)
add_project_dependency(rerun_sdk 0.16.0 REQUIRED)

set(INSTALL_DOCUMENTATION OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH True)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel"
                                               "RelWithDebInfo")
endif()

# Set binary, library and archive directories
function(set_standard_output_directory target)
  set_target_properties(
    ${target}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
               LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
               ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endfunction()

option(BUILD_PYTHON_INTERFACE "Build the Python bindings for this library." ON)
cmake_dependent_option(
  GENERATE_PYTHON_STUBS "Generate stub files for the Python library." ON
  BUILD_PYTHON_INTERFACE OFF)

set(LIB_NAME ${PROJECT_NAME})
file(GLOB lib_SRC CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB lib_HEADERS CONFIGURE_DEPENDS "src/*.hpp")

add_library(${LIB_NAME} SHARED "${lib_SRC}")
set_standard_output_directory(${LIB_NAME})

set(pinocchio_rerun_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(
  ${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${pinocchio_rerun_SRC_DIR}>
                     $<INSTALL_INTERFACE:include>)
set(_build_dir_includes ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME})
target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${_build_dir_includes}>)
target_link_libraries(${LIB_NAME} PUBLIC rerun_sdk)
target_link_libraries(${LIB_NAME} PRIVATE fmt::fmt)
target_link_libraries(${LIB_NAME} PUBLIC assimp::assimp)
target_link_libraries(${LIB_NAME} PUBLIC pinocchio::pinocchio)
target_link_libraries(${LIB_NAME} PUBLIC pinocchio_visualizers::pinocchio_visualizers)

install(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
install(FILES ${lib_HEADERS} DESTINATION include/${PROJECT_NAME})

if(BUILD_PYTHON_INTERFACE)
  add_subdirectory(python)
endif(BUILD_PYTHON_INTERFACE)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
