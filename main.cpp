#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "helpers.h"
#include <iostream>
#include <time.h>
#include <glBoilerplateAndHelpers/sprite.h>
#include "glBoilerplateAndHelpers/texture.h"

std::string guess;
std::string word;
std::string fails;
char last;
int lives = 8;
bool win = false;
class hangman {
public:
    sprite spriteT;
    hangman(GLuint glTexture, float xPos, float yPos, float hVal, float wVal) :
    spriteT(glTexture, xPos, yPos, hVal, wVal) {};
    void changeHangmanTexture(GLuint i) {
        spriteT.setTexture(
            textures::loadTextureToBuffer(
                    std::string("hangman/") + std::to_string(8-i) + ".png", 0, 0)
        );
    }
};
event<unsigned int> liveLost = event<unsigned int>();
void init() {
    word = helpers::get_word();

    std::cout << word << " " << word.size()<< std::endl;

    guess = std::string(word.size(), '_');
    guess.shrink_to_fit();

    std::cout << guess << "" << guess.size() << std::endl;

    lives = 8;
    win = false;
    last = ' ';
    fails.clear();
    liveLost(lives);
}

void save_last(char c) {
    std::cout << c;
    last = c;
}

void confirm() {
    char c = last;

    if (c == ' ' || lives <= 0 || win) return;

    bool gotem = false;
    for (int i = 0; i < word.size(); i++) {
        if (word[i] == c || abs(word[i] - c) == 32) {
            guess[i] = c;
            gotem = true;
        }
    }
    lives += gotem - 1;
    if (!gotem) liveLost(lives);

    if (gotem) {
        win = true;
        for (int i = 0; i < word.size(); i++) {
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


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    unsigned int hangmanBuffer = textures::loadTextureToBuffer(std::string("hangman/0.png"), 0, 0);
    hangman hangman(hangmanBuffer, -.75, -.7, 1.4, 1.4);
    liveLost.add(&hangman::changeHangmanTexture, &hangman);

    glEnable(GL_BLEND);
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
        hangman.spriteT.draw(shaderprog);

        if (lives <= 0)
        {
            reveal.setText("word: " + word);
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
