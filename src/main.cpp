#define GLFW_DLL

#include <stdio.h>
#include <string.h>
#include <cmath>

#include "../headers/GL/glew.h"
#include "../headers/GLFW/glfw3.h"

#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtc/matrix_transform.hpp"
#include "../headers/glm/gtc/type_ptr.hpp"

const GLint WIDTH = 800, HEIGHT = 800;
const float toRadians = 3.14f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformTime;
float moveAngle = 0.0f, xPos = 0.0f, yPos = 0.0f, time = 0.0f;

//Vertex Shader
static const char *vShader = "				                    \
	#version 330							                    \n\
											                    \n\
	layout(location = 0) in vec3 pos;		                    \n\
											                    \n\
	uniform mat4 model;										    \n\
	uniform float time;						                    \n\
											                    \n\
	out vec4 vCol;							                    \n\
											                    \n\
	void main()								                    \n\
	{										                    \n\
		gl_Position = model * vec4(pos, 1.0);			        \n\
		vCol = vec4(clamp(pos, 0.0, 1.0), 1.0);			        \n\
	}										                    \n\
";

//Fragment Shader
static const char *fShader = "				                    \
	#version 330							                    \n\
											                    \n\
	in vec4 gl_FragCoord;										\n\
	in vec4 vCol;							                    \n\
											                    \n\
	out vec4 colour;							                \n\
											                    \n\
	uniform float time;						                    \n\
											                    \n\
	void main()								                    \n\
	{										                    \n\
		colour = vCol;                      					\n\
	}										                    \n\
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

    uniformModel = glGetUniformLocation(shader, "model");
    uniformTime  = glGetUniformLocation(shader, "time");
}

void CreateTriangle() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
       -1.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 0.0f, 
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
            
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

    //Enable Depth Teste
    glEnable(GL_DEPTH_TEST);

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

        moveAngle += 0.001f;
        xPos = cos(moveAngle);
        yPos = sin(moveAngle);
        time = 1;

        //Clear Window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Use the shader program
        __glewUseProgram(shader);

            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
            model = glm::rotate   (model, moveAngle * 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale    (model, glm::vec3(0.2f, 0.2f, 1.0f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1fv(uniformTime, 1, &time);

            //Bind VAO (Vertex Array Objects)
            __glewBindVertexArray(VAO);

                //Bind IBO or EBO (Index Buffer Object / Element Buffer Object)
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

                    //Draw
                    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

                //Unbind IBO / EBO
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            //Unbind VAO
            __glewBindVertexArray(0);

        __glewUseProgram(0);

        //Swap the 2 window buffers
        glfwSwapBuffers(mainWindow);
    }
}