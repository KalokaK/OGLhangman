//
// Created by kaloka on 14/11/2020.
//

#ifndef OGLHANGMAN_HELPERS_H
#define OGLHANGMAN_HELPERS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace helpers {
    void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height);
    void render();
}

namespace input {
    void setupGlfwInputCallbacks(GLFWwindow *window);
    void closeWindowCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
}

namespace shaders
{

    unsigned int load_shader(const char* filename, int shadertype);
}


#endif //OGLHANGMAN_HELPERS_H
