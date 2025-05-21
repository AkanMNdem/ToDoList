cmake_minimum_required(VERSION 3.15)
project(to_do_list_cpp)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include header files
include_directories(include)

# Find required packages
find_package(Drogon REQUIRED)
# find_package(SQLite3 REQUIRED)

# Define source files for library
set(LIB_SOURCES
    src/ToDoList.cpp
    src/TaskPrioritizer.cpp
)

# Create library
add_library(todo_lib STATIC ${LIB_SOURCES})
target_link_libraries(todo_lib PRIVATE SQLite3)

# Backend console app
add_executable(to_do_list_cpp src/main.cpp)
target_link_libraries(to_do_list_cpp PRIVATE todo_lib)

# API server
add_executable(todo_api src/api.cpp)
target_link_libraries(todo_api PRIVATE todo_lib Drogon::Drogon)

