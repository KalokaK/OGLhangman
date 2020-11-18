//
// Created by kaloka on 17/11/2020.
//

#include "sprite.h"

void sprite::draw(unsigned int shaderProgram) {
    if (enabled) {
        glUniform1i(glGetUniformLocation(shaderProgram, "text"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

sprite::sprite(GLuint glTexture, float xPos, float yPos, float hVal, float wVal) : enabled(true), scale(1.0f), texture(), vao(), ebo() {
    x = xPos;
    y = yPos;
    w = wVal;
    h = hVal;
    texture = glTexture;
    generateVertexBuffer();
}

void sprite::onEnable() {

}

void sprite::onDisable() {

}

unsigned int sprite::getTexture() const {
    return texture;
}

bool sprite::isEnabled() const {
    return enabled;
}

void sprite::setTexture(unsigned int i) {
    sprite::texture = i;
}

void sprite::setEnabled(bool val) {
    sprite::enabled = val;
}

void sprite::generateVertexBuffer() {
    float vertices[] = {
            // positions          // colors           // texture coords
            x+w,  y+h, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            x+w, y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            x, y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            x,  y+h, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    ebo = EBO;
    vao = VAO;
}


