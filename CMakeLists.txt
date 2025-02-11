cmake_minimum_required(VERSION 3.10)
project(PUCCHDecoder)

set(CMAKE_CXX_STANDARD 17)

add_executable(PUCCHDecoder src/main.cpp src/Matrix.cpp src/BlockDecoder.cpp src/Orchestrator.cpp)