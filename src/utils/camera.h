#ifndef CAMERA_H
#define CAMERA_H

// --- student code ---

// TODO: get a shape to render

#pragma once
#include <glm/glm.hpp>

class camera {

public:
    glm::mat4 getViewMatrix() const;
    glm::mat4 getInverseViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    float getAspectRatio() const;
    float getHeightAngle() const;
    float getFocalLength() const;
    float getAperture() const;
    glm::vec3 getPosition() const;
    glm::vec3 getLook() const;
    glm::vec3 getUp() const;
    void setCamera(glm::vec3 _pos, glm::vec3 _look, glm::vec3 _up,  float _aperature, float _focalLength, float _heightAngle, float _aspectRatio);
    void update();
    void updateProjection(float _heightAngle, float _aspectRatio);
    void updatePosition(glm::vec3 _deltaPos);
    void updateLook(float _deltaX, float _deltaY);

private:
    glm::vec3 pos;
    glm::vec3 look;
    glm::vec3 up;
    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;
    glm::mat4 projMatrix;
    float aperature;
    float focalLength;
    float heightAngle;
    float aspectRatio;

};


// --- student code end ---

#endif // CAMERA_H
