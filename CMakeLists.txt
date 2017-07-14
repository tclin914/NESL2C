
cmake_minimum_required(VERSION 3.8)

project(NESL2C)

find_package(LLVM REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(include)

add_subdirectory(lib)
add_subdirectory(tools)
