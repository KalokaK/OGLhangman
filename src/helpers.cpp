//
// Created by kaloka on 14/11/2020.
//

#include <glad/glad.h>
#include "helpers.h"
#include <fstream>
#include "calibri.c"

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


namespace sprites
{

    unsigned int Text::textids[100];

    void Text::TextInit()
    {
        glGenTextures(100, textids);
        for (int i = 0; i < 100; i++)
        {
            glBindTexture(GL_TEXTURE_2D, textids[i]);  

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CALIBRI_FRAME_WIDTH, CALIBRI_FRAME_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, calibri_data[i]);
            //glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    Text::Text(std::string text, int posx, int posy)
    {

    }

    void Text::setText(std::string newtext)
    {
        text = newtext;
    }

    std::string Text::getText()
    {
        return text;
    }

    void Text::draw()
    {
        const char* cars = text.c_str();

        for (int i = 0; i < text.size(); i++)
        {
            
        }
    }

};