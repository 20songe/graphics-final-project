#include <stdexcept>
#include "camera.h"
#include <glm/gtx/transform.hpp>

/**
 * @brief Stores information about the camera for future use in ray tracing and
 *        for access throughout the class
 * @param metaData containing information about the camera
 */
Camera::Camera(RenderData metaData, glm::vec4 newLook, glm::vec4 newPos, glm::vec4 newUp, int width, int height, float near, float far) :
    look(glm::normalize(newLook)),
    pos(newPos),
    up(glm::normalize(newUp)),
    heightAngle(metaData.cameraData.heightAngle),
    viewMat(calculateViewMatrix()),
    invViewMat(glm::inverse(viewMat)),
    screenWidth(width),
    screenHeight(height)
{
    setProjectionMatrix(near, far);
}

/**
 * @brief Helper function to get the view matrix of the camera (from Lab 6) by using
 *        stored look, up, and pos vectors.
 * @return view matrix of the camera
 */
glm::mat4 Camera::calculateViewMatrix() {
    glm::vec3 upAsVec3 = glm::vec3(up);
    glm::vec3 w = glm::vec3(-look);
    glm::vec3 v = glm::normalize(upAsVec3 - (glm::dot(upAsVec3, w) * w));
    glm::vec3 u = glm::normalize(glm::cross(glm::vec3(v), glm::vec3(w)));
    glm::mat4 translate = glm::translate(glm::vec3(-pos));
    glm::mat4 rotate = glm::transpose(glm::mat4(glm::mat3(u, v, w)));
    return rotate * translate;
}

void Camera::setViewMatrix() {
    viewMat = calculateViewMatrix();
}

glm::mat4 Camera::getViewMatrix() {
    return viewMat;
}

float Camera::getAspectRatio() const {
    return (float) screenWidth/screenHeight;
}

float Camera::getHeightAngle() const {
    return heightAngle;
}

glm::mat4 Camera::getProjectionMatrix() {
    return proj;
}

void Camera::setProjectionMatrix(float newNear, float newFar) { //can't use glm::perspective
    near = newNear;
    far = newFar;
    float heightAngle = getHeightAngle();
    float tanHalfHeight = tan(heightAngle / 2.0f);
    float widthHalfAngle = getAspectRatio() * tanHalfHeight;
    glm::mat4 result = glm::mat4(0);
    result[0][0] = (float) 1 / (widthHalfAngle);
    result[1][1] = (float) 1 / (tanHalfHeight);
    result[2][2] = - (far + near) / (far - near);
    result[2][3] = - 1.0f;
    result[3][2] = - (float (2) * far * near) / (far - near);
    proj = result;
}
