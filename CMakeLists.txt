cmake_minimum_required(VERSION 3.26)
project(PRiR)

set(CMAKE_CXX_STANDARD 14)

include_directories(exc/ex1)

find_package(MPI REQUIRED)

add_executable(PRiR
        exc/ex1/Alloc.cpp
        exc/ex1/Alloc.h
        exc/ex1/Life.cpp
        exc/ex1/Life.h
        exc/ex1/LifeSequentialImplementation.cpp
        exc/ex1/LifeSequentialImplementation.h
        exc/ex1/Main.cpp
        exc/ex1/Rules.cpp
        exc/ex1/Rules.h
        exc/ex1/SimpleRules.cpp
        exc/ex1/SimpleRules.h)

target_link_libraries(PRiR PRIVATE MPI::MPI_CXX)