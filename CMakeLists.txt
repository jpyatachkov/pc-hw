cmake_minimum_required(VERSION 3.10)
project(pc_hw)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS})
endif()

add_executable(pc_hw main.cpp reducer.hpp transposer.hpp timeutils.hpp measuredelegate.hpp)
