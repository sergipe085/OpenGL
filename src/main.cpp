#define GLFW_DLL
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include "../headers/GL/glew.h"
#include "../headers/GLFW/glfw3.h"

#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtc/matrix_transform.hpp"
#include "../headers/glm/gtc/type_ptr.hpp"

#include "../headers/Window.h"
#include "../headers/Mesh.h"
#include "../headers/Shader.h"
#include "../headers/Camera.h"
#include "../headers/Texture.h"

const float toRadians = 3.14f / 180.0f;

Window* mainWindow;
std::vector<Mesh*>  meshList;
std::vector<Shader> shaders;
Camera camera;

Texture dirtTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime  = 0.0f;

//Vertex Shader
static const char* vShader = "shaders/vert.shader";

//Fragment Shader
static const char* fShader = "shaders/frag.shader";

void CreateObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
    //  x     y     z       u     v
       -1.0f,-1.0f, 0.0f,   0.0f, 0.0f,
        0.0f,-1.0f, 1.0f,   0.5f, 0.0f,
        1.0f,-1.0f, 0.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,   0.5f, 1.0f
    };
    
    GLfloat cubeVertices[] = {
    //   x     y     z      u     v
        -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 1.0f,
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

    // Mesh* obj1 = new Mesh();
    // obj1->CreateMesh(cubeVertices, cubeIndices, 24, 36);
    // meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 20, 12);
    meshList.push_back(obj2);
}

void CreateShaders() {
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaders.push_back(*shader1);
}

int main() {
   
    mainWindow = new Window(800, 600);
    mainWindow->Initialize();

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow->GetBufferWidth() / (GLfloat)mainWindow->GetBufferHeight(), 0.1f, 100.0f);

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.4f);

    dirtTexture = Texture((char*)"textures/dirt.png");
    dirtTexture.LoadTexture();

    GLuint uniformProjection = 0, uniformModel = 0, uniformView;

    CreateObjects();
    CreateShaders();

    //Loop Until Window Close
    while(!mainWindow->GetShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        //Get and Handle user input events
        glfwPollEvents();

        camera.keyControl(mainWindow->GetKeys(), deltaTime);
        camera.mouseControl(mainWindow->GetXChange(), mainWindow->GetYChange());

        //Clear Window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Use the shader program
        shaders[0].UseShader();
        uniformProjection = shaders[0].GetProjectionLocation();
        uniformModel      = shaders[0].GetModelLocation();
        uniformView       = shaders[0].GetViewLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        // model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        // glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        // dirtTexture.UseTexture();
        // meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); //bind the model matrix to the uniform variable on the shader
        dirtTexture.UseTexture();
        meshList[0]->RenderMesh();

        __glewUseProgram(0);

        //Swap the 2 window buffers
        mainWindow->SwapBuffers();
    }
}