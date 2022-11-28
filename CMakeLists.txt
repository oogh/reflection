cmake_minimum_required(VERSION 3.10)
project(reflection)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/Cellar/nlohmann-json/3.10.5/include)

add_executable(example example.cpp)