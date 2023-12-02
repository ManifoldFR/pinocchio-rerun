cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME septum)
set(PROJECT_URL "")
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(pinocchio REQUIRED)
find_package(fmt REQUIRED)

include(FetchContent)

FetchContent_Declare(
  rerun_sdk
  URL https://github.com/rerun-io/rerun/releases/latest/download/rerun_cpp_sdk.zip)
FetchContent_MakeAvailable(rerun_sdk)

add_executable(App src/main.cpp)
set_target_properties(App PROPERTIES CXX_STANDARD 17)

target_link_libraries(App PUBLIC rerun_sdk)
