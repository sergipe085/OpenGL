#define GLFW_DLL

#include <stdio.h>

#include "../headers/Window.h"

#include "../headers/GL/glew.h"
#include "../headers/GLFW/glfw3.h"

Window::Window()
{
    width  = 800;
    height = 600;
    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = false;
    }
}

Window::Window(GLint _width, GLint _height) {
    width  = _width;
    height = _height;
    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = false;
    }
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

    //Handle Key and Mouse Inputs
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks() {
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

GLfloat Window::GetXChange() {
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::GetYChange() {
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

Window::~Window() {

}