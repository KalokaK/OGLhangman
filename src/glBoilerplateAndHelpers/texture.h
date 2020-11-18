//
// Created by kaloka on 18/11/2020.
//

#ifndef OGLHANGMAN_TEXTURE_H
#define OGLHANGMAN_TEXTURE_H
#include <helpers.h>
#include <string>
#include <stb/stb_image.h>

using std::string;
namespace textures {
    unsigned int loadTextureToBuffer(const string &pngSpriteFileLocation);
}


#endif //OGLHANGMAN_TEXTURE_H
