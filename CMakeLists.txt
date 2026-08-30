cmake_minimum_required(VERSION 3.16)
project(simulation)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(SRC 
    shared/gui.cpp
    shared/camera.cpp
    shared/transform.cpp
    shared/control.cpp
    shared/utils.cpp
    shared/shaders.cpp
    shared/geometry.cpp
    shared/mesh.cpp
    shared/physics.cpp
    shared/render.cpp
    shared/material.cpp
    shared/math5d.cpp
    shared/window.cpp
    )

add_subdirectory(third-party/glfw EXCLUDE_FROM_ALL)
add_subdirectory(libs/matrix-exponential EXCLUDE_FROM_ALL)
add_subdirectory(third-party/imgui EXCLUDE_FROM_ALL)

add_library(glad STATIC EXCLUDE_FROM_ALL libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

find_package(glm CONFIG REQUIRED)

add_library(relativity_lib ${SRC})
target_include_directories(relativity_lib PUBLIC include)

target_link_libraries(relativity_lib PUBLIC glfw)
target_link_libraries(relativity_lib PUBLIC glad)
target_link_libraries(relativity_lib PUBLIC expm)
target_link_libraries(relativity_lib PUBLIC m)
target_link_libraries(relativity_lib PUBLIC glm::glm)
target_link_libraries(relativity_lib PUBLIC imgui)

option(BUILD_TESTS "Building tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
else()
    add_executable(simulation src/8-shaders/main.cpp)
    target_link_libraries(simulation PRIVATE relativity_lib)
endif()
