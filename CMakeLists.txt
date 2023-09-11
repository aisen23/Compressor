cmake_minimum_required(VERSION 3.23)

project(as_test_1 VERSION 1.0.0)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD_REQUIRED FALSE)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    include(${CMAKE_CURRENT_SOURCE_DIR}/configs/linux.cmake)
endif()

include_directories(src)
file(GLOB_RECURSE SRC_FILES src/*.h src/*/*.h src/*.cpp src/*/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set(PCH_HEADER src/pch.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADER})

