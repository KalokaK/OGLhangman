#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "helpers.h"
#include <fstream>

const char* vertshader_loc = "vertshader.vert";
const char* fragshader_loc = "fragshader.frag";

int main() {

    // create open gl context //
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // //

    // make glfw window //
    GLFWwindow * window = glfwCreateWindow(800, 600, "hangman", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw "no glfw window created this is bad, check dependencies";
    }
    glfwMakeContextCurrent(window);
    // //

    // set up glad //
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw "glad died something has gone wrong check dependencies";
    }
    // //

    // shaders //
    unsigned int vertshader = shaders::load_shader(vertshader_loc, GL_VERTEX_SHADER);
    unsigned int fragshader = shaders::load_shader(fragshader_loc, GL_FRAGMENT_SHADER);
    // //

    printf("\nshader success!\n");

    // setup input //
    input::setupGlfwInputCallbacks(window, input::inputHandler());
    // //

    // set viewport rect //
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, helpers::framebufferSizeCallback);
    // //

    // main game loop //
    while(!glfwWindowShouldClose(window))
    {
        helpers::render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // //
    return 0;
//
}
