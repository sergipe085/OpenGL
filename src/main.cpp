#define GLFW_DLL

#include <stdio.h>
#include <string.h>

#include "../headers/GL/glew.h"
#include "../headers/GLFW/glfw3.h"

const GLint WIDTH = 800, HEIGHT = 800;

GLuint VAO, VBO, shader;

//Vertex Shader
static const char *vShader = "				\
	#version 330							\n\
											\n\
	layout(location = 0) in vec3 pos;		\n\
											\n\
	uniform mat4 model;						\n\
											\n\
	void main()								\n\
	{										\n\
		gl_Position = vec4(pos * 0.1, 1.0);			\n\
	}										\n\
";

//Fragment Shader
static const char *fShader = "				\
	#version 330							\n\
											\n\
	in vec4 gl_FragCoord;										\n\
											\n\
	out vec4 colour;							\n\
											\n\
	void main()								\n\
	{										\n\
		colour = vec4(gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z, 1.0f);					\n\
	}										\n\
";

void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType) {
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
        printf("Error compiling the %d shader: %s \n", shaderType, eLog);
        return;
    }

    glAttachShader(shaderProgram, shader);
}

void CompileShader() {
    shader = glCreateProgram();

    if (!shader) {
        printf("Error creating shader program!");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking shader program: %s\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating shader program: %s", eLog);
        return;
    }
}

void CreateTriangle() {
    GLfloat vertices[] = {
        0.0f, 1.0f, 0.0f,
       -1.0f,-1.0f, 0.0f, 
        1.0f,-1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

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
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Title", NULL, NULL);
    if (!mainWindow) {
        printf("Error creating window!");
        glfwTerminate();
        return 1;
    }

    //Get buffer size of window
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Set Context to GLEW use
    glfwMakeContextCurrent(mainWindow);

    //Alow modern extension features
    glewExperimental = GL_TRUE;

    //Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("Error initializing GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShader();

    //Loop Until Window Close
    while(!glfwWindowShouldClose(mainWindow)) {
        //Get and Handle user input events
        glfwPollEvents();

        //Clear Window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Use the shader program
        __glewUseProgram(shader);

            //Bind VAO (Vertex Array Objects)
            __glewBindVertexArray(VAO);

                //Draw VAO's
                glDrawArrays(GL_TRIANGLES, 0, 3);

            //Unbind VAO
            __glewBindVertexArray(0);

        __glewUseProgram(0);

        //Swap the 2 window buffers
        glfwSwapBuffers(mainWindow);
    }
}