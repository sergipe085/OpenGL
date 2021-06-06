#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "../headers/GL/glew.h"

#include "../headers/Shader.h"

Shader::Shader() {
    shaderID          = 0;
    uniformModel      = 0;
    uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexPath, const char* fragmentPath) {
    std::string vertexString   = ReadFile(vertexPath);
    std::string fragmentString = ReadFile(fragmentPath);
    
    const char* vertexCode     = vertexString.c_str();
    const char* fragmentCode   = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* filePath) {
    std::string  content;
    std::fstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Filed to open %s! File does not exists!", filePath);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
    shaderID = glCreateProgram();

    if (!shaderID) {
        printf("Error creating shader program");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking shader program: %s\n", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating shader program: %s\n", eLog);
        return;
    }

    uniformModel      = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void Shader::AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glShaderSource(shader, 1, code, codeLength);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: %s\n", shaderType, eLog);
        return;
    }

    glAttachShader(shaderProgram, shader);
}

GLuint Shader::GetProjectionLocation() {
    return uniformProjection;
}

GLuint Shader::GetModelLocation() {
    return uniformModel;
}

void Shader::UseShader() {
    glUseProgram(shaderID);
}

void Shader::ClearShader() {
    if (shaderID) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel      = 0;
    uniformProjection = 0;
}

Shader::~Shader() {
    ClearShader();
}