cmake_minimum_required(VERSION 3.16)
project(simulation)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(third-party/glfw EXCLUDE_FROM_ALL)

add_library(glad STATIC EXCLUDE_FROM_ALL libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

add_executable(simulation src/01-window/main.cpp)
target_include_directories(simulation PRIVATE src/include)

target_link_libraries(simulation PRIVATE glfw)
target_link_libraries(simulation PRIVATE glad)
