#define GLFW_DLL

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include "../headers/GL/glew.h"
#include "../headers/GLFW/glfw3.h"

#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtc/matrix_transform.hpp"
#include "../headers/glm/gtc/type_ptr.hpp"

#include "../headers/Mesh.h"

const GLint WIDTH = 800, HEIGHT = 800;
const float toRadians = 3.14f / 180.0f;

GLuint shaderProgram, uniformModel, uniformProjection, uniformTime;
float moveAngle = 0.0f, xPos = 0.0f, yPos = 0.0f, time = 0.0f;

std::vector<Mesh*> meshList;

//Vertex Shader
static const char *vShader = "				                    \
	#version 330							                    \n\
											                    \n\
	layout(location = 0) in vec3 pos;		                    \n\
											                    \n\
	uniform mat4 model;										    \n\
	uniform mat4 projection;								    \n\
	uniform float time;						                    \n\
											                    \n\
	out vec4 vCol;							                    \n\
											                    \n\
	void main()								                    \n\
	{										                    \n\
		gl_Position = projection * model * vec4(pos, 1.0);	    \n\
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
    shaderProgram = glCreateProgram();

    if (!shaderProgram) {
        printf("Error creating shader program!");
        return;
    }

    AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
    AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
        printf("Error linking shader program: %s\n", eLog);
        return;
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
        printf("Error validating shader program: %s", eLog);
        return;
    }

    uniformModel      = glGetUniformLocation(shaderProgram, "model");
    uniformProjection = glGetUniformLocation(shaderProgram, "projection");
    uniformTime       = glGetUniformLocation(shaderProgram, "time");
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
    
    GLfloat cubeVertices[] = {
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f, 
         0.5f, 0.5f,-0.5f, 
         0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f, 
        -0.5f, 0.5f, 0.5f, 
         0.5f, 0.5f, 0.5f, 
         0.5f,-0.5f, 0.5f
    };

    unsigned int cubeIndices[] = {
    /*FRONT*/ 0, 1, 3, 1, 3, 2,
    /*BACK*/  4, 5, 7, 5, 7, 6,
    /*TOP*/   1, 5, 2, 5, 2, 6,
    /*BOT*/   0, 4, 3, 4, 3, 7,
    /*RIGHT*/ 3, 2, 7, 2, 7, 6,
    /*LEFT*/  4, 5, 0, 5, 0, 1
    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(cubeVertices, cubeIndices, 24, 36);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
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

    //Active vsync
    glfwSwapInterval(1);

    //Alow modern extension features
    glewExperimental = GL_TRUE;

    //Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("Error initializing GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //Enable Depth Teste
    glEnable(GL_DEPTH_TEST);

    //Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

    CreateTriangle();
    CompileShader();

    //Loop Until Window Close
    while(!glfwWindowShouldClose(mainWindow)) {
        //Get and Handle user input events
        glfwPollEvents();

        moveAngle += 0.005f;
        xPos = cos(moveAngle);
        yPos = sin(moveAngle);
        time = 1;

        //Clear Window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Use the shader program
        __glewUseProgram(shaderProgram);

            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform1fv(uniformTime, 1, &time);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
            model = glm::rotate   (model, moveAngle * 100 * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
            model = glm::scale    (model, glm::vec3(0.4f, 0.4f, 0.4f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            meshList[0]->RenderMesh();

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
            model = glm::rotate   (model, moveAngle * 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale    (model, glm::vec3(0.2f, 0.2f, 0.2f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //bind the model matrix to the uniform variable on the shader
            meshList[1]->RenderMesh();

        __glewUseProgram(0);

        //Swap the 2 window buffers
        glfwSwapBuffers(mainWindow);
    }
}