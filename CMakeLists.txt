cmake_minimum_required(VERSION 3.9)
project(keccak)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake
        )

add_library(keccak
        keccak.c
        keccak.h)

add_subdirectory(test)