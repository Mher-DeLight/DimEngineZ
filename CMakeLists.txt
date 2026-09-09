cmake_minimum_required(VERSION 3.15)
project(DimEngineZ)
option(BUILD_TESTS "Build DimEngineZ tests." OFF)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

add_library(DimEngineZ
    src/common.cpp
    src/physics.cpp
    src/manager.cpp
    src/input.cpp)
target_include_directories(DimEngineZ PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(DimEngineZ PRIVATE raylib)

if (BUILD_TESTS)
    add_executable(DimEngineZ_test test/main.cpp)
    target_link_libraries(DimEngineZ_test PRIVATE DimEngineZ)
endif()