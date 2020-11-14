//
// Created by kaloka on 14/11/2020.
//

#ifndef OGLHANGMAN_HELPERS_H
#define OGLHANGMAN_HELPERS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "string"
namespace helpers {
    void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height);
    void render();
}

namespace input {
    struct inputHandler {
    public:
        static bool textmode;
        static std::string currentWordInput;
        inputHandler();
        static void keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void characterCallback(GLFWwindow *window, unsigned int codepoint);

    private:
        static void closeWindowCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void processGenericCharacter(unsigned int codepoint);
    };
    void setupGlfwInputCallbacks(GLFWwindow *window, input::inputHandler handler);
}


#endif //OGLHANGMAN_HELPERS_H
