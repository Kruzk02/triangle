cmake_minimum_required(VERSION 3.16)
project(MyApp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")

# Glad
add_library(glad_c STATIC lib/glad/src/glad.c)
target_include_directories(glad_c PUBLIC lib/glad/include)

# Glfw
add_subdirectory(lib/glfw)

#Opengl
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS src/*.cpp)
add_executable(MyApp ${APP_SRC})

target_include_directories(MyApp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(MyApp PRIVATE
        glad_c
        glfw
        OpenGL::GL
)
