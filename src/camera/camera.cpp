#include <stdexcept>
#include <iostream>
#include "camera.h"
#include "../settings.h"

/**
 * @brief Camera::init - creates camera object
 * @param width - width of scene
 * @param height - height of scene
 * @param metaData - scene render data
 */
void Camera::init(int width, int height) {
    this->c_width = width;
    this->c_height = height;
    this->c_pos = glm::vec4(0,0,0,1);

    this->c_heightAngle = 45.0;

    this->c_look = glm::normalize(glm::vec4(-9, -3.2, -16, 0));
    this->c_up = glm::normalize(glm::vec4(0, 1, 0, 0));

    this->c_left = glm::vec4(glm::normalize(glm::cross(glm::vec3(c_up), glm::vec3(c_look))), 0.f);

    this->c_left = glm::vec4(glm::normalize(glm::cross(glm::vec3(c_up), glm::vec3(c_look))), 0.f);

    // Calculate camera view matrix
    glm::vec4 w = -glm::normalize(c_look);
    glm::vec4 v = glm::normalize(c_up - (glm::dot(c_up, w)*w));
    glm::vec3 u = glm::cross(glm::vec3(v), glm::vec3(w));

    glm::mat4 translate = glm::mat4(
        1.0f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        -this->c_pos[0], -this->c_pos[1], -this->c_pos[2], 1.f);

    this->c_translationMatrix = translate;

    glm::mat4 rotate = glm::mat4(
        u[0], v[0], w[0], 0.f,
        u[1], v[1], w[1], 0.f,
        u[2], v[2], w[2], 0.f,
        0.f, 0.f, 0.f, 1.f);

    this->c_rotationMatrix = rotate;

    this->c_view = rotate * translate;

    this->c_worldY = glm::vec4(0.f, 1.f, 0.f, 0.f);

    updatePerspective();
}

/**
 * @brief Camera::updatePerspective - updates perspective matrix of camera
 * (called when the near/far plane or the scene changes)
 */
void Camera::updatePerspective() {
    float c = -settings.nearPlane / settings.farPlane;
    float f = settings.farPlane;
    float tanh = glm::tan(this->getHeightAngle() / 2.f);
    float ar = 1 / this->getAspectRatio();
    float tanw = tanh * ar;

    glm::mat4 remap(1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, -2.f, 0.f,
                    0.f, 0.f, -1.f, 1.f);
    glm::mat4 unhinge(1.f, 0.f, 0.f, 0.f,
                      0.f, 1.f, 0.f, 0.f,
                      0.f, 0.f, 1.f/(1.f + c), -1.f,
                      0.f, 0.f, -c / (1.f + c), 0.f);
    glm::mat4 scale(1.f / (f * tanw), 0.f, 0.f, 0.f,
                    0.f, 1.f / (f * tanh), 0.f, 0.f,
                    0.f, 0.f, 1.f / f, 0.f,
                    0.f, 0.f, 0.f, 1.f);

    this->c_perspective = remap * unhinge * scale;
}

/**
 * @brief Camera::getPerspectiveMatrix - getter for perspective matrix
 * @return the perspective matrix
 */
glm::mat4 Camera::getPerspectiveMatrix() const {
    return c_perspective;
}

/**
 * @brief Camera::getViewMatrix - calculate camera view matrix
 * @return view matrix of camera
 */
glm::mat4 Camera::getViewMatrix() const {
    return c_view;
}

/**
 * @brief Camera::getAspectRatio - getter for aspect ratio
 * @return aspect ratio (height / width)
 */
float Camera::getAspectRatio() const {
    return float(c_height) / float(c_width);
}

void Camera::updateHeight(float height) {
    c_height = height;
}

void Camera::updateWidth(float width) {
    c_width = width;
}

/**
 * @brief Camera::getHeightAngle - getter for height angle
 * @return height angle in radians
 */
float Camera::getHeightAngle() const {
    return this->c_heightAngle;
}

/**
 * @brief Camera::getPos - getter for camera position in world space
 * @return coordinates (4D) of camera position as a point
 */
glm::vec4 Camera::getPos() const {
    return this->c_pos;
}

/**
 * @brief Camera::getFocalLength - camera focal length getter
 * @return camera focal length
 */
float Camera::getFocalLength() const {
    return this->c_focalLength;
}

/**
 * @brief Camera::getAperture - getter for camera aperture
 * @return camera aperture
 */
float Camera::getAperture() const {
    return this->c_aperture;
}

void Camera::translate(glm::vec4 direction) {
    this->c_pos += direction;
    glm::mat4 translate = glm::mat4(
        1.0f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        -this->c_pos[0], -this->c_pos[1], -this->c_pos[2], 1.f);

    this->c_translationMatrix = translate;
    this->c_view = this->c_rotationMatrix * this->c_translationMatrix;
}

glm::vec4 Camera::getLook() {
    return this->c_look;
}

glm::vec4 Camera::getUp() {
    return this->c_up;
}

glm::vec4 Camera::getLeft() {
    return this->c_left;
}

glm::vec4 Camera::getWorldY() {
    return this->c_worldY;
}

/**
 * @brief Camera::rotate
 * @param theta - nagle in radians
 * @param axis - axis to rotate about
 * @return
 */
void Camera::rotate(float theta, glm::vec3 axis) {
    glm::vec3 normalizedAxis = glm::normalize(axis);
    float ux = normalizedAxis[0];
    float uy = normalizedAxis[1];
    float uz = normalizedAxis[2];

    glm::mat4 newRotation(
        glm::cos(theta) + pow(ux, 2) * (1 - glm::cos(theta)), ux * uy * (1 - glm::cos(theta)) + uz * glm::sin(theta), ux * uz * (1 - glm::cos(theta)) - uy * glm::sin(theta), 0.f,
        ux * uy * (1 - glm::cos(theta)) - uz * glm::sin(theta), glm::cos(theta) + pow(uy, 2) * (1 - glm::cos(theta)), uy * uz * (1 - glm::cos(theta)) + ux * glm::sin(theta), 0.f,
        ux * uz * (1 - glm::cos(theta)) + uy * glm::sin(theta), uy * uz * (1 - glm::cos(theta)) - ux * glm::sin(theta), glm::cos(theta) + pow(uz, 2) * (1 - glm::cos(theta)), 0.f,
        0.f, 0.f, 0.f, 1.f);

    this->c_rotationMatrix = newRotation * this->c_rotationMatrix;
    this->c_view = this->c_rotationMatrix * this->c_translationMatrix;
    this->c_look = newRotation * this->c_look;

    this->c_left = newRotation * this->c_left;

    glm::vec4 w = -glm::normalize(c_look);
    glm::vec4 v = glm::normalize(c_up - (glm::dot(c_up, w)*w));
    glm::vec3 u = glm::cross(glm::vec3(v), glm::vec3(w));

    glm::mat4 rotate = glm::mat4(
        u[0], v[0], w[0], 0.f,
        u[1], v[1], w[1], 0.f,
        u[2], v[2], w[2], 0.f,
        0.f, 0.f, 0.f, 1.f);

    this->c_rotationMatrix = rotate;

    this->c_view = rotate * c_translationMatrix;


}
