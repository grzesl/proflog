cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME "proflog-app")
project(${PROJECT_NAME} C CXX)
add_subdirectory(src)
include_directories(./src)
add_executable(${PROJECT_NAME} 
    app.c
) 
target_link_libraries(${PROJECT_NAME} proflog)