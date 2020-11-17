#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "helpers.h"
#include <fstream>
#include <iostream>
#include <time.h>

std::string guess;
std::string word;
std::string fails;
sprites::Text* buff;
char last;
int lives = 8;
bool win = false;

void outprinter(char c)
{
    buff->setText(buff->getText() + c);
}

void save_last(char c)
{
    std::cout << c;
    last = c;
}

void confirm()
{
    char c = last;
    if (c == ' ' || lives <= 0 || win) return;

    bool gotem = false;
    for (int i = 0; i < word.size(); i++)
    {
        if (word[i] == c || abs(word[i] - c) == 32)
        { 
            guess[i] = c;
            gotem = true;
        }
    }

    lives += gotem - 1;

    if (gotem)
    {
        win = true;
        for (int i = 0; i < word.size(); i++)
        {
            win = win && (guess[i] != '_');
        }
    }

    last = ' ';
    printf("called\n");
}

void back()
{
    std::string str = buff->getText();
    str.erase(str.end() - 1);


    buff->setText(str);
}

std::string get_word()
{
    // reads a random word out of nouns.txt and returns it
    static std::ifstream nouns("nouns.txt");

    std::string result;
    int lim;

    srand(time(0));

    lim = rand() % 4554;

    while (lim--> 0)
    {
            nouns >> result;
    }

    nouns.close();
    return result;
}

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

    handler.inputActionHolder.genericCharacterEvent.add(save_last);
    handler.inputActionHolder.accept.add(confirm);

    sprites::Text::TextInit();

    word = get_word();
    printf(word.c_str());


    for (int i = 0; i < word.size(); i++) guess.append("_");

    sprites::Text text(guess, -1, .75, 0.13);
    sprites::Text g("guess: " + last, -1, .6, 0.09);
    sprites::Text health("lives: " + std::to_string(lives), 0.27, -1, 0.09);
    sprites::Text gameOver("GAME OVER", -1, -.111111111111, .2222222222222);
    sprites::Text congrats("CORRECT", -1, -.142842842842, 0.2857142857142);
    sprites::Text reveal("word: " + word, -1, -.3, .1);

    

    // main game loop //
    while(!glfwWindowShouldClose(window))
    {
        helpers::render();
        auto str = g.getText();
        str[str.size() - 1] = last;
        g.setText(str);
        text.setText(guess);
        health.setText("lives: " + std::to_string(lives));

        text.draw();
        g.draw();
        health.draw();

        if (lives <= 0)
        {
            gameOver.draw();
            reveal.draw();
        } 

        if (win)
        {
            congrats.draw();
            reveal.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // //
    return 0;
//
}
