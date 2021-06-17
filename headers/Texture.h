#pragma once

#include "../headers/GL/glew.h"

#include "../headers/stb_image.h"

class Texture
{
public:
    Texture();
    Texture(char* _fileLocation);

    void LoadTexture();
    void UseTexture();
    void ClearTexture();

    ~Texture();

private: 
    GLuint textureID;
    int width, height, bitDepth;

    char* fileLocation;
};