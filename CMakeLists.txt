cmake_minimum_required(VERSION 3.16)
project(MyApp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")

add_subdirectory(vendored/glfw EXCLUDE_FROM_ALL)

add_executable(MyApp src/main.cpp src/glad.c)
target_include_directories(MyApp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(MyApp
    PRIVATE glfw
)
