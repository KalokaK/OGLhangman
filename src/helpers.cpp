//
// Created by kaloka on 14/11/2020.
//

#include <glad/glad.h>
#include "helpers.h"

namespace helpers {
    void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

namespace input {
    void setupGlfwInputCallbacks(GLFWwindow *window, inputHandler handler) {
        // keypressCallback //
        glfwSetKeyCallback(window, input::inputHandler::keypressCallback);
        // //

        // textCallback //
        glfwSetCharCallback(window, input::inputHandler::characterCallback);
        // //
    }

    void inputHandler::keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_END:
                switch (action) {
                    case GLFW_PRESS:
                        closeWindowCallback(window, key, scancode, action, mods);
                        break;
                    default:
                        ;
                }
                break;
            default:
                ;
        }
    }

    void inputHandler::characterCallback(GLFWwindow *window, unsigned int codepoint) {
        if (65 <= codepoint && codepoint <= 90) {
            processGenericCharacter(codepoint);
        } else if (97 <= codepoint && codepoint <= 122) {
            processGenericCharacter(codepoint - 32);
        } else if (codepoint == 32) {
            processGenericCharacter(32);
        }
    }

    inputHandler::inputHandler() = default;

    void inputHandler::closeWindowCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        glfwSetWindowShouldClose(window, true);
    }

    void inputHandler::processGenericCharacter(unsigned int codepoint) {

    }
}
