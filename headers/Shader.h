#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "../headers/GL/glew.h"

class Shader{
public:
    Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles (const char* vertexPath, const char* fragmentPath);

    std::string ReadFile(const char* filePath);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    GLuint GetViewLocation();

    void UseShader();
    void ClearShader();

    ~Shader();
private:
    GLuint shaderID, uniformProjection, uniformModel, uniformView;

    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType);
};