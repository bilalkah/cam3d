cmake_minimum_required(VERSION 3.20)
project(cam3d LANGUAGES CXX)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add debug flags and treat warnings as errors for include and src directories
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64-v3")
endif()

# External folder for third-party packages
find_package(Git REQUIRED)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "Git not found. Please install Git.")
endif()

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

# SDL3 setup
set(SDL3_DIR ${EXTERNAL_DIR}/SDL3)
if(NOT EXISTS ${SDL3_DIR})
    message(STATUS "SDL3 submodule not found. Updating...")
    execute_process(
        COMMAND git submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

add_subdirectory(${SDL3_DIR} EXCLUDE_FROM_ALL)

set(SDL3_INCLUDE_DIRS ${SDL3_DIR}/include)
set(SDL3_LIBRARIES ${PROJECT_BINARY_DIR}/${SDL3_DIR})

include_directories(
    ${SDL3_INCLUDE_DIRS}
    include
)

link_directories(${SDL3_LIBRARIES})

add_executable(
    cam3d_example 
    src/main.cpp
    src/rasterizer.cpp
    src/algorithm.cpp
)
target_include_directories(cam3d_example PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(cam3d_example PRIVATE SDL3::SDL3)