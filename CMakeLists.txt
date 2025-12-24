cmake_minimum_required(VERSION 3.16)
project(graphic)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")

# Glad
add_library(glad_c STATIC lib/glad/src/glad.c)
target_include_directories(glad_c PUBLIC lib/glad/include)

# Glfw
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

#Opengl
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS src/*.cpp)
add_executable(graphic ${APP_SRC})

add_custom_command(TARGET graphic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/asset
        $<TARGET_FILE_DIR:graphic>/asset
)

target_include_directories(graphic
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(graphic PRIVATE
        glad_c
        glfw
        OpenGL::GL
        glm::glm
)
