#pragma once

#include <stdio.h>

#include "../headers/GL/glew.h"
#include "../headers/GLFW/glfw3.h"

class Window {
public:
    Window();
    Window(GLint width, GLint height);

    int Initialize();

    GLint GetBufferWidth()  { return bufferWidth;  }
    GLint GetBufferHeight() { return bufferHeight; }

    bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

    void SwapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();
private:
    GLFWwindow* mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;
};