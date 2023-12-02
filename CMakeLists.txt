cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME septum)
set(PROJECT_URL "")
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(pinocchio REQUIRED)
find_package(assimp REQUIRED)
find_package(fmt 10.0 REQUIRED)

include(FetchContent)

set(BUILD_SHARED_LIBS ON)

FetchContent_Declare(
  rerun_sdk
  URL https://github.com/rerun-io/rerun/releases/latest/download/rerun_cpp_sdk.zip)
FetchContent_MakeAvailable(rerun_sdk)

option(BUILD_TESTING "Build tests" ON)

set(LIB_NAME ${PROJECT_NAME})
file(GLOB lib_SRC CONFIGURE_DEPENDS "src/*.cpp")

message(STATUS "Library sources: ${lib_SRC}")
add_library(${LIB_NAME} SHARED "${lib_SRC}")

target_include_directories(${LIB_NAME} PUBLIC src/)
target_link_libraries(${LIB_NAME} PUBLIC rerun_sdk)
target_link_libraries(${LIB_NAME} PRIVATE fmt::fmt)
target_link_libraries(${LIB_NAME} PUBLIC pinocchio::pinocchio)

add_subdirectory(pyseptum)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
