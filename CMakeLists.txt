cmake_minimum_required (VERSION 3.15)

project(openmldb-external-udfs)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  simdjson
  GIT_REPOSITORY https://github.com/simdjson/simdjson.git
  GIT_TAG  tags/v3.2.1
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(simdjson)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${simdjson_SOURCE_DIR}/include
)

add_library(json_array_sort json_array_sort.cc)
target_link_libraries(json_array_sort simdjson)

add_library(list_at list_at.cc)

add_executable(func-test func-test.cc)
target_link_libraries(func-test json_array_sort list_at)
