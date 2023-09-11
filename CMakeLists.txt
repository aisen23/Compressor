cmake_minimum_required(VERSION 3.23)

project(as_test_1 VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Comment this lines when build Release version.
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(DEBUG_BUILD)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Uncomment this lines when build Release version.
#add_compile_definitions(NDEBUG)

include_directories(src)
file(GLOB_RECURSE SRC_FILES src/*.h src/*/*.h src/*.cpp src/*/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set(PCH_HEADER src/pch.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADER})