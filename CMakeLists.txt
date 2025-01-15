cmake_minimum_required(VERSION 3.15)
project(to_do_list_cpp)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include header files
include_directories(include)

# Add the executable and link the source files
find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE unofficial::sqlite3::sqlite3)
add_executable(to_do_list_cpp src/main.cpp src/ToDoList.cpp)