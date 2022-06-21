cmake_minimum_required(VERSION 3.0.0)
project(TSP VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)

# aux_source_directory(./src SRC_FILES)
add_executable(TSP_test src/TSP.cpp src/main_test.cpp)
add_executable(TSP_tsplib src/TSP.cpp src/main_tsplib.cpp)
add_executable(TSP_app src/TSP.cpp src/main.cpp)
