cmake_minimum_required(VERSION 3.3)

project(tests)

enable_testing()

include(gtest)


file(COPY data DESTINATION .)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTEST_LIBRARY} libkeccak)
add_test(${PROJECT_NAME} ${PROJECT_NAME} )