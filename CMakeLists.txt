cmake_minimum_required(VERSION 3.9)
project(libkeccak)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake
)


set(MUST_HAVE_COMPILER_FLAGS "-fno-strict-aliasing -msse2")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${MUST_HAVE_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MUST_HAVE_COMPILER_FLAGS}")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${MUST_HAVE_COMPILER_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${MUST_HAVE_COMPILER_FLAGS}")


include_directories(include)

add_library(libkeccak
        src/keccak.c
        include/keccak/keccak.h)


add_subdirectory(test ./tests/${PROJECT_NAME} EXCLUDE_FROM_ALL)
