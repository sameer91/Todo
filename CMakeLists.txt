cmake_minimum_required(VERSION 3.22)

project(Todo)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Debug = ON")
  add_definitions(-DDEBUG)
endif()

include_directories(include)

add_compile_options(
  -Wall
  -Wextra
  -Wshadow
  -Wdouble-promotion
  -Wformat=2
  -Werror=format-security
  -Wundef
  -fno-common
  -Wconversion
  -Werror=implicit-function-declaration)

add_executable(testRun test/Test.cpp src/todo.cpp src/Utils.cpp)
