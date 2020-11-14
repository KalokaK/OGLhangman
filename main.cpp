#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    // create open gl context //
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // //
    return 0;

    // make glfw window //
    GLFWwindow * glfwWindow = glfwCreateWindow(800, 600, "hangman", nullptr, nullptr);
}