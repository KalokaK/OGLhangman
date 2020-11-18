//
// Created by kaloka on 17/11/2020.
//

#ifndef OGLHANGMAN_SPRITE_H
#define OGLHANGMAN_SPRITE_H
#include <helpers.h>
#include <string>
#include <stb/stb_image.h>
#include <filesystem>

using std::string;
class sprite : public sprites::Sprite {
    unsigned int vao;
    unsigned int texture;
    bool enabled;
    unsigned int ebo;
public:
    void setTexture(unsigned int i);
    void setEnabled(bool val);
    [[nodiscard]] unsigned int getTexture() const;
    [[nodiscard]] bool isEnabled() const;
private:
    void onEnable();
    void onDisable();
    void generateVertexBuffer();
    void generateTexture(const string &pngSpriteFileLocation, float posX, float posY);
public:
    float scale;
    explicit sprite(GLuint glTexture, float xPos, float yPos, float hVal, float wVal);

    void draw(unsigned int shaderProgram) override;
};


#endif //OGLHANGMAN_SPRITE_H
