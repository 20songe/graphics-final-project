#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "utils/sceneparser.h"

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:
    // Constructor
    void init(int width, int height, const RenderData &metaData);

    // Update camera matrices
    void updatePerspective();

    // Get perspective matrix
    glm::mat4 getPerspectiveMatrix() const;

    // Returns the view matrix for the current camera settings.
    glm::mat4 getViewMatrix() const;

    // Returns the position of the camera in world space
    glm::vec4 getPos() const;
    glm::mat4 getNormalMatrix() const;

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    void updateWidth(float width);
    void updateHeight(float height);

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;

    // action
    void translatePos();

    // action
    void updateViewMatrixLook(float direction);
    void updateViewMatrixRightLeft(float direction);
    void updateViewMatrixWorld(float direction);

    void translate(glm::vec4 direction);
    void rotate(float theta, glm::vec3 axis);

    glm::vec4 getUp();
    glm::vec4 getLook();
    glm::vec4 getLeft();
    glm::vec4 getWorldY();

private:
    // Fields
    int c_width;
    int c_height;
    float c_heightAngle;
    float c_focalLength;
    float c_aperture;
    glm::mat4 c_view;
    glm::vec4 c_pos;
    glm::vec4 c_look;
    glm::vec4 c_up;
    glm::vec4 c_left;
    glm::vec4 c_worldY;
    glm::mat4 c_perspective;
    glm::mat4 c_rotationMatrix;
    glm::mat4 c_translationMatrix;
};
