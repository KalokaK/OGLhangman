//
// Created by kaloka on 14/11/2020.
//

#include <glad/glad.h>
#include "helpers.h"
#include <fstream>

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
    void setupGlfwInputCallbacks(GLFWwindow *window) {
        // close window key binds //
        glfwSetKeyCallback(window, keypressCallback);
        // //
    }

    void closeWindowCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        glfwSetWindowShouldClose(window, true);
    }

    void keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
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

        int  success;
        glGetShaderiv(out, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(out, 512, NULL, infoLog);
            printf("compilation of  %s  failed\n%s\n\n%s\n", filename, infoLog, code);
            delete[] code;
            throw "shader compilation failed";
        }
        delete[] code;

        return out;
    }


    unsigned int shader_program()
    {
        unsigned int shader[2];

        unsigned int out = glCreateProgram();
        shader[0] = load_shader(vertshader_loc, GL_VERTEX_SHADER);
        glAttachShader(out, shader[0]);
        shader[1] = load_shader(fragshader_loc, GL_FRAGMENT_SHADER);
        glAttachShader(out, shader[1]);
        glLinkProgram(out);

        glDeleteShader(shader[0]);
        glDeleteShader(shader[1]);

        int success;
        glGetProgramiv(out, GL_LINK_STATUS, &success);

        if(!success) {
            char infoLog[512];
            glGetProgramInfoLog(out, 512, NULL, infoLog);
            printf("shaderporgramm linking didn't work: %s\n", infoLog);
            throw "ShaderPorgram didn't work";
        }

        return out;
    }

}
