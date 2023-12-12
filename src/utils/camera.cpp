
// --- student code ---

#include "camera.h"
#include "settings.h"

#include <iostream>

glm::mat4 camera::getViewMatrix() const {
    return viewMatrix;
}

glm::mat4 camera::getInverseViewMatrix() const {
    return inverseViewMatrix;
}

glm::mat4 camera::getProjectionMatrix() const {
    return projMatrix;
}

float camera::getAspectRatio() const {
    // height / width
    // width : height

    return aspectRatio;
}

float camera::getHeightAngle() const {
    return heightAngle;
}

float camera::getFocalLength() const {
    return focalLength;
}

float camera::getAperture() const {
    return aperature;
}

glm::vec3 camera::getPosition() const {
    return pos;
}

glm::vec3 camera::getLook() const {
    return look;
}

glm::vec3 camera::getUp() const {
    return up;
}

// calculations

glm::mat4 getTranslationMatrix(float dx, float dy, float dz) {
    return glm::mat4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        dx, dy, dz, 1.f
    );
}

glm::mat4 calcViewMatrix(glm::vec3 pos, glm::vec3 look, glm::vec3 up) {

    auto T = getTranslationMatrix(-pos[0], -pos[1], -pos[2]);

    auto w = -look / glm::length(look);
    auto v = glm::normalize(up - (glm::dot(up, w) * w));
    auto u = glm::cross(v, w);

    glm::mat4 R (
        u[0], v[0], w[0], 0.f,
        u[1], v[1], w[1], 0.f,
        u[2], v[2], w[2], 0.f,
        0.f, 0.f, 0.f, 1.f
        );
    return R * T;

}

glm::mat4 calcProjectionMatrix(float heightAngle, float aspectRatio) {

    glm::mat4 M0 {
        1.f, 0.f,  0.f, 0.f,
        0.f, 1.f,  0.f, 0.f,
        0.f, 0.f, -2.f, 0.f,
        0.f, 0.f, -1.f, 1.f
    };

    auto c = -settings.nearPlane / settings.farPlane;

    glm::mat4 M1 {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f / (1.f + c), -1.f,
        0.f, 0.f, -c / (1.f + c), 0.f
    };

    auto tanH = glm::tan(heightAngle / 2.f);
    auto tanW = aspectRatio * tanH;

    glm::mat4 M2 {
        1.f / (settings.farPlane * tanW), 0.f, 0.f, 0.f,
        0.f, 1.f / (settings.farPlane * tanH), 0.f, 0.f,
        0.f, 0.f, 1.f / settings.farPlane, 0.f,
        0.f, 0.f, 0.f, 1.0f
    };

    return M0 * M1 * M2;
}

void camera::setCamera(glm::vec3 _pos, glm::vec3 _look, glm::vec3 _up, float _aperature, float _focalLength, float _heightAngle, float _aspectRatio) {
    pos = _pos;
    look = normalize(_look);
    up = normalize(_up);
    aperature = _aperature;
    focalLength = _focalLength;
    heightAngle = _heightAngle;
    aspectRatio = _aspectRatio;

    viewMatrix = calcViewMatrix(_pos, _look, _up);
    inverseViewMatrix = glm::inverse(viewMatrix);
    projMatrix = calcProjectionMatrix(heightAngle, aspectRatio);
}

void camera::update() {
    projMatrix = calcProjectionMatrix(heightAngle, aspectRatio);
}

void camera::updateProjection(float _heightAngle, float _aspectRatio) {
    heightAngle = _heightAngle;
    aspectRatio = _aspectRatio;
    projMatrix = calcProjectionMatrix(_heightAngle, _aspectRatio);
}

void camera::updatePosition(glm::vec3 _deltaPos) {
    pos = pos + _deltaPos;
    viewMatrix = calcViewMatrix(pos, look, up);
    inverseViewMatrix = glm::inverse(viewMatrix);
}

glm::mat3 rotation(float angle, glm::vec3 axis) {

    return glm::mat3{
        glm::cos(angle) + axis[0] * axis[0] * (1 - glm::cos(angle)),
        axis[0] * axis[1] * (1 - glm::cos(angle)) + axis[2] * glm::sin(angle),
        -(axis[0] * axis[2] * (1 - glm::cos(angle)) + axis[1] * glm::sin(angle)),

        axis[0] * axis[1] * (1 - glm::cos(angle)) - axis[2] * glm::sin(angle),
        glm::cos(angle) + axis[1] * axis[1] * (1 - glm::cos(angle)),
        axis[1] * axis[2] * (1 - glm::cos(angle)) + axis[0] * glm::sin(angle),

        axis[0] * axis[2] * (1 - glm::cos(angle)) + axis[1] * glm::sin(angle),
        axis[1] * axis[2] * (1 - glm::cos(angle)) - axis[0] * glm::sin(angle),
        glm::cos(angle) + axis[2] * axis[2] * (1 - glm::cos(angle))
    };

}

void camera::updateLook(float _deltaX, float _deltaY) {
    look = normalize(rotation(_deltaX * 1e-3f, glm::vec3(0.f, 1.f, 0.f)) * look);
    look = normalize(rotation(_deltaY * 1e-3f, glm::normalize(glm::cross(look, up))) * look);
    viewMatrix = calcViewMatrix(pos, look, up);
    inverseViewMatrix = glm::inverse(viewMatrix);
}


// --- student code end ---
