#pragma once

#include "../headers/GL/glew.h"
#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtc/matrix_transform.hpp"

#include "../headers/GLFW/glfw3.h"

class Camera {
public:

    Camera();
    Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed);

    void keyControl(bool* keys);

    glm::mat4 calculateViewMatrix();

    ~Camera();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat turnSpeed;

    void update();
};