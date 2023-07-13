cmake_minimum_required (VERSION 3.13)

project(openmldb-external-udfs)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

set(CMAKE_CXX_STANDARD 17)

add_library(simdjson include/simdjson.cpp)

add_library(json_array_sort json_array_sort.cc)
target_link_libraries(json_array_sort simdjson)

add_library(list_at list_at.cc)

add_executable(func-test func-test.cc)
target_link_libraries(func-test json_array_sort list_at)
