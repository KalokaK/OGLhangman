//
// Created by kaloka on 14/11/2020.
//

#ifndef OGLHANGMAN_HELPERS_H
#define OGLHANGMAN_HELPERS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
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
    constexpr const char* vertshader_loc = "vertshader.vert";
    constexpr const char* fragshader_loc = "fragshader.frag";
    
    unsigned int load_shader(const char* filename, int shadertype);
    unsigned int shader_program();
}

namespace sprites
{
    class Sprite
    {
    public:
        float x;
        float y;
        float w;
        float h;

        virtual void draw() = 0;
    };

    class Text: public Sprite
    {
    public:

        static unsigned int textids[100];
        static bool initflag;

        static unsigned int VBO, VAO, EBO;

        static void TextInit();
        
        Text();

        Text(std::string text, float posx = 0, float posy = 0, float height = 0.05);
        void setText(std::string newtext);
        std::string getText();

        void draw();

    private:
        std::string text = "";

    };

};


#endif //OGLHANGMAN_HELPERS_H
