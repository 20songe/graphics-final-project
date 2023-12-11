#define CAMERA_H
#include <glm/glm.hpp>

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:
    glm::vec4 look;
    glm::vec4 pos;
    glm::vec4 up;
    float heightAngle;
    glm::mat4 viewMat;
    glm::mat4 invViewMat;
    glm::mat4 proj;
    int screenWidth;
    int screenHeight;
    float near = 0.1f;
    float far = 10.0f;
    Camera() = default;
    Camera(glm::vec4 newLook, glm::vec4 newPos, glm::vec4 newUp, int width, int height, float near, float far);

    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    glm::mat4 getViewMatrix();
    glm::mat4 calculateViewMatrix();
    void setViewMatrix();

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    glm::mat4 getProjectionMatrix();
    void setProjectionMatrix(float near, float far);

};
