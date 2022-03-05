cmake_minimum_required(VERSION 3.22)

project(Todo)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("Debug = ON")
	add_definitions(-DDEBUG)
endif()

add_executable(test_app main.cpp todo.cpp )

include_directories(./)
