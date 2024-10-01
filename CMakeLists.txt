cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g -O0")

project(JumpRabbit)

set(CMAKE_BUILD_TYPE Release)

find_package(OpenGL REQUIRED) # Graphics Library
find_package(GLEW REQUIRED) # Automatically manages OpenGL extensions
find_package(glm CONFIG REQUIRED) # Math library for OpenGL
find_package(SDL2 CONFIG REQUIRED) # Provides OpenGL context, and input event handling
find_package(assimp REQUIRED) # Asset importing library

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(JumpRabbit ${SOURCE})
target_link_libraries(JumpRabbit PRIVATE  ${OPENGL_LIBRARIES} SDL2::SDL2 glm::glm GLEW::GLEW assimp::assimp)
target_include_directories(JumpRabbit PRIVATE ${OPENGL_INCLUDE_DIRECTORIES})
