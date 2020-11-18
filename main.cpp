#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "helpers.h"
#include <iostream>
#include <time.h>

std::string guess;
std::string word;
std::string fails;
char last;
int lives = 8;
bool win = false;

void init()
{
    word = helpers::get_word();

    std::cout << word << " " << word.size()<< std::endl;

    guess = std::string(word.size(), '_');
    guess.shrink_to_fit();

    std::cout << guess << "" << guess.size() << std::endl;

    lives = 8;
    win = false;
    last = ' ';
    fails.clear();
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
    else
    {
        bool in = false;
        for (int i = 0; i < fails.size(); i++) in = in || fails[i] == c;

        if (!in)
        {
            if (fails.size() > 0) fails.push_back(',');
            fails.push_back(c);
        }
    }
    

    last = ' ';
    printf("called\n");
}

int main(int argc, char* argv[]) {

    srand(time(0));
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
    handler.inputActionHolder.back.add(init);

    init();

    sprites::Text::TextInit();

    sprites::Text text(guess, -1, .8, 0.12);
    sprites::Text g("guess: ", -1, .65, 0.09);
    sprites::Text health("lives: " + std::to_string(lives), 0.27, -1, 0.09);
    sprites::Text gameOver("GAME OVER", -1, -.111111111111, .2222222222222);
    sprites::Text congrats("CORRECT", -1, -.14, 0.2857142857142);
    sprites::Text reveal("word: " + word, -1, -.3, .1);
    sprites::Text next("start a new round by pressing backspace", -1, -.4, 0.051282);
    sprites::Text misses("miss:" + fails, -1, -.9, .07);

    

    // main game loop //
    while(!glfwWindowShouldClose(window))
    {
        helpers::render();
        g.setText(g.getText().replace(g.getText().size() - 1, 1, 1, last));
        text.setText(guess);
        health.setText("lives: " + std::to_string(lives));
        misses.setText("miss:" + fails);

        text.draw(0);
        g.draw(0);
        health.draw(0);
        misses.draw(0);

        if (lives <= 0)
        {
            gameOver.draw(0);
            reveal.draw(0);
            next.draw(0);
        } 

        if (win)
        {
            congrats.draw(0);
            next.draw(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // //
    return 0;
//
}
