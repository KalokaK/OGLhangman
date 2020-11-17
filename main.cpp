#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "helpers.h"
#include <fstream>

int main(int argc, char* argv[]) {

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
    unsigned int shaderprog = shaders::shader_program();
    glUseProgram(shaderprog);
    // //

    printf("\nshader success!\n");

    // setup input //
    auto handler = input::inputHandler();
    input::setupGlfwInputCallbacks(window, &handler);
    // //

    // set viewport rect //
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, helpers::framebufferSizeCallback);
    // //

    sprites::Text::TextInit();

    sprites::Text one("Hello_World", -1, 0, 0.09);
    input::inputHandler inputs;


    // main game loop //
    while(!glfwWindowShouldClose(window))
    {
        helpers::render();
        one.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // //
    return 0;
//
}
