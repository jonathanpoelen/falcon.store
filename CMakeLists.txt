project(Falcon.Store)

cmake_minimum_required(VERSION 2.8)

include(CMakeDefinitions.txt)
include_directories(.)
#add_definitions(-g)

add_executable(store_set_test test/store_set_test.cpp)

# add_subdirectory(tests)

# enable_testing()
