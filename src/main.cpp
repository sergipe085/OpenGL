#define GLFW_DLL

#include <stdio.h>

//#include "libs/GLEW/include/GL/glew.h"
#include "../headers/GLFW/glfw3.h"

const GLint WIDTH = 800, HEIGHT = 800;

int main() {
    
    //Initialize GLFW
    if (!glfwInit()) {
        printf("Error initializing GLFW!");
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
    glfwCreateWindow(WIDTH, HEIGHT, "Title", NULL, NULL);
}