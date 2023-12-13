#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include "GL/glew.h" // Must always be first include
#include <QOpenGLWidget>
#include "glm/glm.hpp"
#include "debug.h";

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLRenderer : public QOpenGLWidget
{
public:
    GLRenderer(QWidget *parent = nullptr);
    ~GLRenderer();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL()                  override; // Called once at the start of the program
    void paintGL()                       override; // Called every frame in a loop
    void resizeGL(int width, int height) override; // Called when window size changes

    void mousePressEvent(QMouseEvent *e) override; // Used for camera movement
    void mouseMoveEvent(QMouseEvent *e)  override; // Used for camera movement
    void wheelEvent(QWheelEvent *e)      override; // Used for camera movement
    void rebuildMatrices();                        // Used for camera movement

private:
    GLuint m_shader;     // Stores id of shader program
    GLuint m_obj_vbo; // Stores id of vbo
    GLuint m_obj_vao; // Stores id of vao
//    GLuint createTextureAttachment(int width, int height);
    void makeFBO();
    void paintTexture(GLuint texture);
    std::vector<float> m_objData;

    void timerEvent(QTimerEvent *event) override;

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames
    float m_time; // stores current time
    QElapsedTimer m_frameTimer; //add one to prevent messing up the m_elapsedTimer

    glm::mat4 m_model = glm::mat4(1);
    glm::mat4 m_view  = glm::mat4(1);

    glm::mat4 m_reflect = glm::mat4(1,0,0,0,
                                    0,-1,0,0,
                                    0,0,1,0,
                                    0,0,0,1) * m_view;

    // Device Correction Variables
    int m_devicePixelRatio;
    std::string const DUDV_MAP = "resources/waterDUDV.png";

    // FBO fields
    GLuint m_defaultFBO;
    int m_screen_width;
    int m_screen_height;
    int m_fbo_width;
    int m_fbo_height;
    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;
    GLuint m_reflection_fbo;
    GLuint m_reflection_texture;
    GLuint m_reflection_renderbuffer;
    GLuint m_texture_shader;

    glm::vec4 center  = glm::vec4(-3.0, 0.0, 5.0, 1.0);
    bool should_ripple = false;
    GLuint m_dudv_texture;
    GLuint m_normal_texture;
    QImage m_image1;
    QImage m_image2;

    GLuint m_refraction_fbo, m_refraction_texture, m_refraction_renderbuffer;


    glm::mat4 m_inv_view = inverse(glm::mat4(1));
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
