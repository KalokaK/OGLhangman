//
// Created by kaloka on 18/11/2020.
//

#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include <exception>

unsigned int textures::loadTextureToBuffer(const string &pngSpriteFileLocation, int iX, int iY) {
    unsigned int text;
    glGenTextures(1, &text);
    glBindTexture(GL_TEXTURE_2D, text);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(pngSpriteFileLocation.c_str(), &width, &height, &nrChannels, 0);
    printf(pngSpriteFileLocation.c_str());
    printf("\nnch: %i ch1: %i, ch2: %i, ch3: %i, ch4%i\n", nrChannels, data[0], data[1], data[2], data[3]);
    if (data)
    {
        for (int i = 0; i < width * height; i++)
        {
            if (data[nrChannels* i + 3] == 0) for(int j = 0; j < nrChannels - 1; j++) data[nrChannels*i + j] = 100;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        throw std::exception();
    }
    stbi_image_free(data);
    return text;
}
