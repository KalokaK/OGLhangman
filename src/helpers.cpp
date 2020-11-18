//
// Created by kaloka on 14/11/2020.
//

#include <glad/glad.h>
#include "helpers.h"
#include <fstream>
#include <time.h>
#include "calibri.c"
#include "functional"

namespace helpers {
    void framebufferSizeCallback(GLFWwindow *glfwWindow, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    std::string get_word()
    {
        // reads a random word out of nouns.txt and returns it
        std::ifstream nouns("nouns.txt");

        std::string result;
        int lim;

        lim = rand() % 4554;

        while (lim--> 0)
        {
                nouns >> result;
        }

        nouns.close();

        return result;
    }
}

namespace input {
    void setupGlfwInputCallbacks(GLFWwindow *window, inputHandler *handler) {
        if (!inputHandler::glfwBound) {
            glfwSetKeyCallback(window, inputHandler::keypressEventCaller);
            glfwSetCharCallback(window, inputHandler::characterEventCaller);
        }
    }

    void inputHandler::keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_UP:
                inputActionHolder.move4Axis(0., 1.);
                break;
            case GLFW_KEY_DOWN:
                inputActionHolder.move4Axis(0., -1.);
                break;
            case GLFW_KEY_RIGHT:
                inputActionHolder.move4Axis(1., 0.);
                break;
            case GLFW_KEY_LEFT:
                inputActionHolder.move4Axis(-1., 0.);
                break;
            case GLFW_KEY_ENTER:
                inputActionHolder.accept();
                break;
            case GLFW_KEY_HOME:
                inputActionHolder.reload();
            case GLFW_KEY_END:
                inputActionHolder.close();
                break;
            case GLFW_KEY_BACKSPACE:
            if (action == GLFW_RELEASE) inputActionHolder.back();
                break;
            default:
                ;
        }
    }

    void inputHandler::characterCallback(GLFWwindow *window, unsigned int codepoint) {
        if (65 <= codepoint && codepoint <= 90) {
            preprocessGenericCharacter(codepoint);
        } else if (97 <= codepoint && codepoint <= 122) {
            preprocessGenericCharacter(codepoint - 32);
        } else if (codepoint == 32) {
            preprocessGenericCharacter(32);
        }
    }

    inputHandler::inputHandler() :
    inputActionHolder() {
        inputHandler::keypressEvent.add(&inputHandler::keypressCallback, this);
        inputHandler::characterEvent.add(&inputHandler::characterCallback, this);
    };
    bool inputHandler::glfwBound = false;
    event<GLFWwindow *, int, int, int, int> inputHandler::keypressEvent =
            event<GLFWwindow *, int, int, int, int>();
    event<GLFWwindow *, unsigned int> inputHandler::characterEvent =
            event<GLFWwindow *, unsigned int>();

    inputHandler::~inputHandler() {
        inputHandler::keypressEvent.sub(&inputHandler::keypressCallback, this);
        inputHandler::characterEvent.sub(&inputHandler::characterCallback, this);
    }

    void inputHandler::preprocessGenericCharacter(unsigned int codepoint) {
        char letter = 63; // char("?")
        if (65 <= codepoint && codepoint <= 90) {
            letter = (char)codepoint;
        } else if (97 <= codepoint && codepoint <= 122) {
            letter = (char)(codepoint - 32);
        } else if (codepoint == 32) {
            letter = (char)32;
        }
        inputActionHolder.genericCharacterEvent(letter);
    }

    void inputHandler::keypressEventCaller(GLFWwindow *window, int key, int scancode, int action, int mods) {
        keypressEvent(window, key, scancode, action, mods);
    }

    void inputHandler::characterEventCaller(GLFWwindow *window, unsigned int codepoint) {
        characterEvent(window, codepoint);
    }

    inputActionEventsHolder::inputActionEventsHolder() :
    move4Axis(),
    accept(),
    reload(),
    genericCharacterEvent(),
    close()
    {}
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

    unsigned int Text::VBO, Text::VAO, Text::EBO;

    unsigned int Text::textids[CALIBRI_FRAME_COUNT];
    bool Text::initflag = false;

    void Text::TextInit()
    {
        if (initflag) return;

        glGenTextures(CALIBRI_FRAME_COUNT, textids);
        for (int i = 0; i < CALIBRI_FRAME_COUNT; i++)
        {
            glBindTexture(GL_TEXTURE_2D, textids[i]);  

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CALIBRI_FRAME_WIDTH, CALIBRI_FRAME_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, calibri_data[i]);
            //glGenerateMipmap(GL_TEXTURE_2D);
        }

        unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
        };  

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        initflag = true;
    }

    Text::Text()
    {
        if (!initflag)
        {
            printf("\nTextsystem not initialized!\n");
            TextInit();
        } 
        x = 0;
        y = 0;

        h = 0.05;
        w = text.size() * h;

        text = "";
    }

    Text::Text(std::string text, float posx, float posy, float height): text(text)
    {
        if (!initflag)
        {
            printf("\nTextsystem not initialized!\n");
            TextInit();
        } 

        h = height;
        x = posx;
        y = posy;
        w = text.size() * h;
    }

    void Text::setText(std::string newtext)
    {
        text = newtext;
        w = text.size() * h;
    }

    std::string Text::getText()
    {
        return text;
    }

    void Text::draw(unsigned int shader)
    {
        float vertices[] = {
        // positions          // colors           // texture coords
        x + h,  y + h, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
        x + h,      y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
            x,      y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
            x,  y + h, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
        };
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


        for (int i = 0; i < text.size(); i++)
        {
            if(text[i] == '\n' || vertices[0] > 1)
            {
                vertices[0] = x + h;
                vertices[8] = x + h;
                vertices[16] = x;
                vertices[24] = x;

                vertices[1] -= h;
                vertices[9] -= h;
                vertices[17] -= h;
                vertices[25] -= h;

                if (text[i] == '\n') continue;
            }
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
            glBindTexture(GL_TEXTURE_2D, textids[text[i]]);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            vertices[0] += h;
            vertices[8] += h;
            vertices[16] += h;
            vertices[24] += h;
        }
    }

};