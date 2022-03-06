cmake_minimum_required(VERSION 3.22)

project(Todo)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("Debug = ON")
	add_definitions(-DDEBUG)
endif()

include_directories(include)
add_executable(test_app src/main.cpp src/todo.cpp src/Utils.cpp)
