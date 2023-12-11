#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "GL/glew.h" // Must always be first include
#include <QOpenGLWidget>
#include "glm/glm.hpp"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLRenderer : public QOpenGLWidget
{
public:
    GLRenderer(QWidget *parent = nullptr);
    ~GLRenderer();

protected:
    void initializeGL()                  override; // Called once at the start of the program
    void paintGL()                       override; // Called every frame in a loop
    void resizeGL(int width, int height) override; // Called when window size changes

    //added helpers
    void initializeReflectionFBO();
    void initializeRefractionFBO();
    void bindReflectionFBO();
    void bindRefractionFBO();
    void unbindCurrentFBO();
    void checkFBOStatus();
    GLuint createTextureAttachment(int width, int height);
    GLuint createDepthTextureAttachment(int width, int height);
    GLuint createDepthBufferAttachment(int width, int height);

    void mousePressEvent(QMouseEvent *e) override; // Used for camera movement
    void mouseMoveEvent(QMouseEvent *e)  override; // Used for camera movement
    void wheelEvent(QWheelEvent *e)      override; // Used for camera movement
    void rebuildMatrices();                        // Used for camera movement

private:
    GLuint m_shader;     // Stores id of shader program
    GLuint m_texture_shader;
    GLuint m_tree_vbo; // Stores id of vbo
    GLuint m_tree_vao; // Stores id of vao

    //water related variables
    GLuint m_water_vbo;
    GLuint m_water_vao;
    glm::mat4 m_waterModelMatrix = glm::mat4(1);

    // FBO properties
    void makeFBO();
    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;
    GLuint m_defaultFBO = 3;
    float m_devicePixelRatio;
    GLuint m_fbo;
    GLuint reflectionFBO;
    GLuint reflectionTexture;
    GLuint reflectionDepthBuffer;

    GLuint refractionFBO;
    GLuint refractionTexture;
    GLuint refractionDepthTexture;

    // FBO dimensions
    const unsigned int REFLECTION_WIDTH = 320;
    const unsigned int REFLECTION_HEIGHT = 180;
    const unsigned int REFRACTION_WIDTH = 1280;
    const unsigned int REFRACTION_HEIGHT = 720;

    //tree related variables
    std::vector<float> m_treeData;
    glm::mat4 m_model = glm::mat4(1);
    glm::mat4 m_view  = glm::mat4(1);
    glm::mat4 m_proj  = glm::mat4(1);

    glm::vec4 m_lightPos; // The world-space position of the point light

    float m_ka;
    float m_kd;
    float m_ks;
    float m_shininess;


    QPoint m_prevMousePos;
    float  m_angleX;
    float  m_angleY;
    float  m_zoom;
};
