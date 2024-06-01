cmake_minimum_required(VERSION 3.16)
project(FinalProject)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(FinalProject main.cpp
        vec3.h
        color.h
        ray.h
        material.h
        quad.h
)
