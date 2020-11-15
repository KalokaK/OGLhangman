//
// Created by kaloka on 14/11/2020.
//

#include <glad/glad.h>
#include "helpers.h"
#include <fstream>
#include "functional"

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
            case GLFW_KEY_UP:
                inputActionEventsHolder::move4Axis(0., 1.);
                break;
            case GLFW_KEY_DOWN:
                inputActionEventsHolder::move4Axis(0., -1.);
                break;
            case GLFW_KEY_RIGHT:
                inputActionEventsHolder::move4Axis(1., 0.);
                break;
            case GLFW_KEY_LEFT:
                inputActionEventsHolder::move4Axis(-1., 0.);
                break;
            case GLFW_KEY_ENTER:
                inputActionEventsHolder::accept();
                break;
            case GLFW_KEY_HOME:
                inputActionEventsHolder::reload();
            case GLFW_KEY_END:
                closeWindowCallback(window, key, scancode, action, mods);
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

    inputHandler::inputHandler()
    {
        inputActionEvents = inputActionEventsHolder();

    };

    void inputHandler::closeWindowCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        glfwSetWindowShouldClose(window, true);
    }

    void inputHandler::processGenericCharacter(unsigned int codepoint) {
        inputActionEventsHolder::genericCharacterEvent((char) codepoint);
    }
}

namespace shaders
{
    unsigned int load_shader(const char* filename, int shadertype)
    {
        std::ifstream file(filename, std::ios::in | std::ios::binary);

        file.seekg(0, file.end);

        int len = file.tellg();

        file.seekg(file.beg);

        char* code = new char[len + 1];

        file.read(code, len);

        code[len] = '\0';

        unsigned int out = glCreateShader(shadertype);

        glShaderSource(out, 1, &code, NULL);

        glCompileShader(out);

        // printf(filename);
        // printf(" :\n");
        // printf(code);
        // printf("\n");

        file.close();
        delete[] code;

        int  success;
        glGetShaderiv(out, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(out, 512, NULL, infoLog);
            printf("compilation of  %s  failed\n%s\n", filename, infoLog);
            throw "shader compilation failed";
        }

        return out;

    }
}
