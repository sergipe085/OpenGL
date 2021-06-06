#define GLFW_DLL

#include <stdio.h>

#include "../headers/Window.h"

#include "../headers/GL/glew.h"
#include "../headers/GLFW/glfw3.h"

Window::Window()
{
    width  = 800;
    height = 600;
}

Window::Window(GLint _width, GLint _height) {
    width  = _width;
    height = _height;
}

int Window::Initialize() {
    //Initialize GLFW
    if (!glfwInit()) {
        printf("Error initializating glfw!");
        glfwTerminate();
        return 1;
    }

    //Setup GLFW window properties
    //Setup OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Core Profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Allow Forward Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create Window
    mainWindow = glfwCreateWindow(width, height, "TITLE", NULL, NULL);
    if (!mainWindow) {
        printf("Error creating window!");
        glfwTerminate();
        return 1;
    }

    //Get buffer size
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Make a context to glew use
    glfwMakeContextCurrent(mainWindow);

    //Active vsync
    glfwSwapInterval(1);

    //Alow modern extension features
    glewExperimental = GL_TRUE;

    //Init glew
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        printf("Error initializating glew");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //enable depth test
    glEnable(GL_DEPTH_TEST);

    //setup glew viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
}

Window::~Window() {

}