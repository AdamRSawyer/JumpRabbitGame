cmake_minimum_required(VERSION 3.8)

set(CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g -O0")
set (CMAKE_CXX_FLAGS_RELEASE "")

project(JumpRabbit)


find_package(assimp CONFIG REQUIRED) # Asset importing library
find_package(OpenGL REQUIRED) # Graphics Library
find_package(GLEW REQUIRED) # Automatically manages OpenGL extensions
find_package(glm CONFIG REQUIRED) # Math library for OpenGL
find_package(SDL2 CONFIG REQUIRED) # Provides OpenGL context, and input event handling


file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(JumpRabbit ${SOURCE})
target_link_libraries(JumpRabbit PRIVATE  assimp::assimp ${OPENGL_LIBRARIES} SDL2::SDL2 glm::glm GLEW::GLEW )
target_include_directories(JumpRabbit PRIVATE ${OPENGL_INCLUDE_DIRECTORIES})
