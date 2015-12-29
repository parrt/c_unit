cmake_minimum_required(VERSION 3.2)
project(cunit)

set(MODULE_NAME cunit)
set(SOURCE cunit.c)

add_library(${MODULE_NAME} ${SOURCE})
