cmake_minimum_required(VERSION 3.2)
project(c_unit_testing)

set(SOURCE c_unit.c)

add_library(c_unit c_unit.c)
target_include_directories (c_unit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(demo sample_test.c)
target_link_libraries(demo LINK_PUBLIC c_unit)
