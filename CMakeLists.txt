cmake_minimum_required(VERSION 3.16)
project(OGLhangman)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/glfw)
include_directories(include)
add_subdirectory(test/Google_tests)
include_directories(src)

set(SOURCE_FILES
        main.cpp
        src/glad.c
        src/helpers.cpp
        src/glBoilerplateAndHelpers/sprite.cpp
        src/glBoilerplateAndHelpers/sprite.h src/glBoilerplateAndHelpers/texture.cpp src/glBoilerplateAndHelpers/texture.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(Google_Tests_run PRIVATE src)

# Linking GLFW and OGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

set(WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/src)

file(COPY hangman DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY hangman DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)

# shaders 
configure_file(src/vertshader.vert Debug/vertshader.vert COPYONLY)
configure_file(src/fragshader.frag Debug/fragshader.frag COPYONLY)
configure_file(nouns.txt Debug/nouns.txt COPYONLY)