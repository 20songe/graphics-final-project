#include <stdexcept>
#include "camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

/**
 * @brief Stores information about the camera for future use in ray tracing and
 *        for access throughout the class
 * @param metaData containing information about the camera
 */
Camera::Camera(glm::vec4 newLook, glm::vec4 newPos, glm::vec4 newUp, int width, int height, float near, float far) :
    look(glm::normalize(newLook)),
    pos(newPos),
    up(glm::normalize(newUp)),
    viewMat(calculateViewMatrix()),
    invViewMat(glm::inverse(viewMat)),
    screenWidth(width),
    screenHeight(height)
{
    setProjectionMatrix(near, far);
    heightAngle = 45.0;
    pitch = calculatePitch();
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

float Camera::calculatePitch() {
    // Get a normalized copy of the look vector
    glm::vec3 lookNorm = glm::normalize(glm::vec3(look));

    // Project the look vector onto the XZ plane (by zeroing out the y component)
    glm::vec3 lookXZProj = glm::normalize(glm::vec3(lookNorm.x, 0.0f, lookNorm.z));

    // Calculate the angle in radians between the original look vector and its projection onto the XZ plane
    float pitchRadians = glm::orientedAngle(lookXZProj, lookNorm, glm::vec3(1.0f, 0.0f, 0.0f));

    // Convert radians to degrees if necessary
    float pitchDegrees = glm::degrees(pitchRadians);

    return pitchDegrees;
}

void Camera::setPitch(float pitch) {
    this->pitch = pitch;

    // Calculate the camera's right vector by crossing the look and up vectors
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(look), glm::vec3(up)));

    // Rotate the look vector around the right vector by the change in pitch
    glm::vec3 newLook = glm::vec3(look);
    newLook = glm::rotate(newLook, pitch - glm::asin(newLook.y), right);

    // Update the look vector of the camera
    look = glm::vec4(newLook, 0.0);

    // Recalculate the view matrix with the new look vector
    viewMat = calculateViewMatrix();
}

void Camera::setViewMatrix() {
    viewMat = calculateViewMatrix();
}

void Camera::setPosition(glm::vec4 position) {
    pos = position;
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

