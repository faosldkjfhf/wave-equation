cmake_minimum_required(VERSION 3.10)
project(wave-equation VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)

find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(main main.cpp libraries/glad/src/glad.c src/pipeline.cpp src/shader.cpp src/engine.cpp)

include_directories(main include)
include_directories(main libraries/glad/include)

target_link_libraries(main glfw Eigen3::Eigen OpenGL::GL)
