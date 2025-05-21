cmake_minimum_required(VERSION 3.15)
project(to_do_list_cpp)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use -std=c++17 instead of -std=gnu++17

# Set export compile commands for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Option to enable testing (ON by default)
option(BUILD_TESTING "Build the testing tree." ON)

# Option to enable/disable building the main API with Drogon
option(BUILD_API "Build the main API with Drogon" ON)

# Include FetchContent
include(FetchContent)

# Include header files
include_directories(include)

# Find OS-specific settings
if(APPLE)
    # On macOS, ensure we use the Drogon installed via Homebrew
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
    
    # Use installed Drogon on macOS
    find_package(Drogon REQUIRED)
else()
    # On Linux/Docker, fetch and build Drogon
    if(BUILD_API)
        find_package(Drogon QUIET)
        if(NOT Drogon_FOUND)
            message(STATUS "Drogon not found, fetching from GitHub")
            # Set trantor version to match Drogon's dependency
            FetchContent_Declare(
                trantor
                GIT_REPOSITORY https://github.com/an-tao/trantor.git
                GIT_TAG v1.5.23
            )
            
            # Fetch and build Drogon
            FetchContent_Declare(
                drogon
                GIT_REPOSITORY https://github.com/drogonframework/drogon.git
                GIT_TAG v1.9.5
            )
            
            # Make them available
            FetchContent_MakeAvailable(trantor drogon)
        endif()
    endif()
endif()

# Find sqlite3
find_package(SQLite3 REQUIRED)
find_package(jsoncpp REQUIRED)

# Define source files for library
set(LIB_SOURCES
    src/ToDoList.cpp
    src/TaskPrioritizer.cpp
)

# Create library
add_library(todo_lib STATIC ${LIB_SOURCES})
target_link_libraries(todo_lib PRIVATE jsoncpp sqlite3)

# Backend console app
add_executable(to_do_list_cpp src/main.cpp)
target_link_libraries(to_do_list_cpp PRIVATE todo_lib)

# API server (only if BUILD_API is enabled)
if(BUILD_API)
    add_executable(todo_api src/api.cpp)
    if(APPLE)
        target_link_libraries(todo_api PRIVATE 
            todo_lib 
            Drogon::Drogon 
            jsoncpp
        )
    else()
        target_link_libraries(todo_api PRIVATE 
            todo_lib 
            drogon
            jsoncpp
        )
    endif()
endif()

# Simple API server for Docker (no external dependencies)
add_executable(todo_api_simple src/api_docker.cpp)
target_link_libraries(todo_api_simple PRIVATE todo_lib)

# ==============================================
# Testing Configuration
# ==============================================

if(BUILD_TESTING)
    # Enable testing
    enable_testing()

    # Include GoogleTest
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Find CURL for API tests
    find_package(CURL REQUIRED)

    # Core tests
    add_executable(task_tests tests/core/TaskTests.cpp)
    target_link_libraries(task_tests PRIVATE todo_lib GTest::gtest GTest::gtest_main)

    # Existing ToDoList tests
    add_executable(todo_list_tests tests/ToDoListTests.cpp)
    target_link_libraries(todo_list_tests PRIVATE todo_lib GTest::gtest GTest::gtest_main)

    # API tests
    add_executable(api_tests tests/api/ApiTests.cpp)
    target_link_libraries(api_tests PRIVATE todo_lib GTest::gtest GTest::gtest_main CURL::libcurl)

    # Add tests to CTest
    add_test(NAME TaskTests COMMAND task_tests)
    add_test(NAME ToDoListTests COMMAND todo_list_tests)
    add_test(NAME ApiIntegrationTests COMMAND api_tests)

    # Make a custom target to run all tests
    add_custom_target(run_tests
      COMMAND ${CMAKE_CTEST_COMMAND} --verbose
      DEPENDS task_tests todo_list_tests api_tests
    )
endif()

