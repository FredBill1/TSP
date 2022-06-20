cmake_minimum_required(VERSION 3.0.0)
project(TSP VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,268435456")

# aux_source_directory(./src SRC_FILES)
add_executable(TSP src/TSP.cpp src/main.cpp)
add_executable(TSP_dp src/TSP_dp.cpp)
