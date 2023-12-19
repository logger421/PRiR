cmake_minimum_required(VERSION 3.27)
project(ex1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

find_package(MPI REQUIRED)

add_executable(ex1
        Alloc.cpp
        Alloc.h
        Life.cpp
        Life.h
        LifeParallelImplementation.cpp
        LifeParallelImplementation.h
        Main.cpp
        Rules.cpp
        Rules.h
        SimpleRules.cpp
        SimpleRules.h)

target_link_libraries(ex1 PRIVATE MPI::MPI_CXX)
