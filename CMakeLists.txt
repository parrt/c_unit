cmake_minimum_required(VERSION 3.2)
project(c_unit_testing)

INCLUDE(CTest)

set(SOURCE c_unit.c)

add_library(c_unit c_unit.c)
target_include_directories (c_unit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sample_test sample_test.c)
target_link_libraries(sample_test LINK_PUBLIC c_unit)

enable_testing()

set(CMAKE_BUILD_TYPE Debug)

ADD_TEST(NAME sample_test
         COMMAND    valgrind --error-exitcode=1 --tool=memcheck --leak-check=full ./sample_test )
