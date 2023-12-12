#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "utils/camera.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

// --- student code ---

#include "utils/objloader.h"
#include "particlesystem.h"

// --- student code end ---

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged(int selection);
    void settingsChanged();
    void saveViewportImage(std::string filePath);

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;


    // --- student code ---

    // final project

    int m_numWaterPoints;
    float m_waterPointElapsedTimes[8];
    glm::vec4 m_waterPointCenters[8];

    particlesystem m_generator;

    objloader m_objloader;

    QImage m_image;

    std::vector<float> m_meshData0;
    std::vector<float> m_meshData1;
    std::vector<float> m_meshData2;
    std::vector<float> m_meshData3;
    std::vector<float> m_meshData4;
    std::vector<float> m_meshData5;
    std::vector<float> m_meshData6;
    std::vector<float> m_meshData7;

    GLuint m_mesh_vbo0;
    GLuint m_mesh_vbo1;
    GLuint m_mesh_vbo2;
    GLuint m_mesh_vbo3;
    GLuint m_mesh_vbo4;
    GLuint m_mesh_vbo5;
    GLuint m_mesh_vbo6;
    GLuint m_mesh_vbo7;

    GLuint m_mesh_vao0;
    GLuint m_mesh_vao1;
    GLuint m_mesh_vao2;
    GLuint m_mesh_vao3;
    GLuint m_mesh_vao4;
    GLuint m_mesh_vao5;
    GLuint m_mesh_vao6;
    GLuint m_mesh_vao7;

    GLuint m_texture0;
    GLuint m_texture1;
    GLuint m_texture2;
    GLuint m_texture3;
    GLuint m_texture4;
    GLuint m_texture5;
    GLuint m_texture6;
    GLuint m_texture7;

    GLuint m_normal_texture0;
    GLuint m_normal_texture1;
    GLuint m_normal_texture2;
    GLuint m_normal_texture3;
    GLuint m_normal_texture4;
    GLuint m_normal_texture5;
    GLuint m_normal_texture6;
    GLuint m_normal_texture7;

    GLuint m_opacity_texture0;
    GLuint m_opacity_texture1;
    GLuint m_opacity_texture2;
    GLuint m_opacity_texture3;
    GLuint m_opacity_texture4;
    GLuint m_opacity_texture5;
    GLuint m_opacity_texture6;
    GLuint m_opacity_texture7;

    // old project
    int m_prevParam0;
    int m_prevParam1;
    float m_prevParam2;
    float m_prevParam3;
    bool m_prevParam4;
    bool m_prevParam5;
    bool m_prevParam6;
    bool m_prevParam7;

    GLuint m_defaultFBO;

    camera m_camera;

    GLuint m_shader;
    GLuint m_texture_shader;

    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;

    void makeFBO();
    int m_fbo_width;
    int m_fbo_height;
    int m_screen_width;
    int m_screen_height;
    GLuint m_fbo;
    GLuint m_fbo_texture;
    GLuint m_fbo_renderbuffer;

    bool m_initialized = false;

    int m_numDirLights;
    int m_numPointLights;
    int m_numSpotLights;

    glm::vec4 m_dirLightDirs[8];
    glm::vec4 m_dirLightColors[8];

    glm::vec4 m_pointLightPos[8];
    glm::vec4 m_pointLightColors[8];
    glm::vec3 m_pointLightAttenuation[8];

    glm::vec4 m_spotLightPos[8];
    glm::vec4 m_spotLightDirs[8];
    glm::vec4 m_spotLightColors[8];
    glm::vec3 m_spotLightAttenuation[8];
    float m_spotLightAngle[8];
    float m_spotLightP[8];

    // --- student code end ---

};
