cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME septum)
set(PROJECT_URL "")
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(pinocchio REQUIRED)
find_package(fmt REQUIRED)
