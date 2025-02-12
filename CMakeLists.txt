cmake_minimum_required(VERSION 3.10)
project(PUCCH_F2)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES src/*.cpp)

add_executable(main ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
else()
    message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(main stdc++fs)
else()
    message(WARNING "This project has not been tested with compilers other than GNU")
endif()
