#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"

// ================== Project 5: Lights, Camera

// --- student code ---

// project 6: todo

#include "utils/shaderloader.h"
#include "utils/objloader.h"

// --- student code end ---

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    // --- student code ---

    // clearing data
    glDeleteBuffers(1, &m_mesh_vbo0);
    glDeleteBuffers(1, &m_mesh_vbo1);
    glDeleteBuffers(1, &m_mesh_vbo2);
    glDeleteBuffers(1, &m_mesh_vbo3);
    glDeleteBuffers(1, &m_mesh_vbo4);
    glDeleteBuffers(1, &m_mesh_vbo5);
    glDeleteBuffers(1, &m_mesh_vbo6);
    glDeleteBuffers(1, &m_mesh_vbo7);

    glDeleteVertexArrays(1, &m_mesh_vao0);
    glDeleteVertexArrays(1, &m_mesh_vao1);
    glDeleteVertexArrays(1, &m_mesh_vao2);
    glDeleteVertexArrays(1, &m_mesh_vao3);
    glDeleteVertexArrays(1, &m_mesh_vao4);
    glDeleteVertexArrays(1, &m_mesh_vao5);
    glDeleteVertexArrays(1, &m_mesh_vao6);
    glDeleteVertexArrays(1, &m_mesh_vao7);

    glDeleteProgram(m_shader);
    glDeleteProgram(m_texture_shader);
    glDeleteVertexArrays(1, &m_fullscreen_vao);
    glDeleteBuffers(1, &m_fullscreen_vbo);
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    // --- student code end ---

    this->doneCurrent();
}

// --- student code ---

void Realtime::makeFBO(){
    // Task 19: Generate and bind an empty texture, set its min/mag filter interpolation, then unbind
    glGenTextures(1, &m_fbo_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Task 20: Generate and bind a renderbuffer of the right size, set its format, then unbind
    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Task 18: Generate and bind an FBO
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Task 21: Add our texture as a color attachment, and our renderbuffer as a depth+stencil attachment, to our FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    // Task 22: Unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);

}

GLuint Realtime::selectTexture(int selection) {
    switch (selection) {
    case 0: {
        return m_texture0;
        break;
    }
    case 1: {
        return m_texture1;
        break;
    }
    case 2: {
        return m_texture2;
        break;
    }
    case 3: {
        return m_texture3;
        break;
    }
    case 4: {
        return m_texture4;
        break;
    }
    case 5: {
        return m_texture5;
        break;
    }
    case 6: {
        return m_texture6;
        break;
    }
    case 7: {
        return m_texture7;
        break;
    }
    default: {
        return m_texture0;
        break;
    }
    }
}

GLuint Realtime::selectOpacityTexture(int selection) {
    switch (selection) {
    case 0: {
        return m_opacity_texture0;
        break;
    }
    case 1: {
        return m_opacity_texture1;
        break;
    }
    case 2: {
        return m_opacity_texture2;
        break;
    }
    case 3: {
        return m_opacity_texture3;
        break;
    }
    case 4: {
        return m_opacity_texture4;
        break;
    }
    case 5: {
        return m_opacity_texture5;
        break;
    }
    case 6: {
        return m_opacity_texture6;
        break;
    }
    case 7: {
        return m_opacity_texture7;
        break;
    }
    default: {
        return m_opacity_texture0;
        break;
    }
    }
}

GLuint Realtime::selectNormalTexture(int selection) {
    switch (selection) {
    case 0: {
        return m_normal_texture0;
        break;
    }
    case 1: {
        return m_normal_texture1;
        break;
    }
    case 2: {
        return m_normal_texture2;
        break;
    }
    case 3: {
        return m_normal_texture3;
        break;
    }
    case 4: {
        return m_normal_texture4;
        break;
    }
    case 5: {
        return m_normal_texture5;
        break;
    }
    case 6: {
        return m_normal_texture6;
        break;
    }
    case 7: {
        return m_normal_texture7;
        break;
    }
    default: {
        return m_normal_texture0;
        break;
    }
    }
}

std::vector<float> Realtime::selectMeshData(int selection) {
    switch (selection) {
    case 0: {
        return m_meshData0;
        break;
    }
    case 1: {
        return m_meshData1;
        break;
    }
    case 2: {
        return m_meshData2;
        break;
    }
    case 3: {
        return m_meshData3;
        break;
    }
    case 4: {
        return m_meshData4;
        break;
    }
    case 5: {
        return m_meshData5;
        break;
    }
    case 6: {
        return m_meshData6;
        break;
    }
    case 7: {
        return m_meshData7;
        break;
    }
    default: {
        return m_meshData0;
        break;
    }
    }
}

GLuint Realtime::selectVbo(int selection) {
    switch (selection) {
    case 0: {
        return m_mesh_vbo0;
        break;
    }
    case 1: {
        return m_mesh_vbo1;
        break;
    }
    case 2: {
        return m_mesh_vbo2;
        break;
    }
    case 3: {
        return m_mesh_vbo3;
        break;
    }
    case 4: {
        return m_mesh_vbo4;
        break;
    }
    case 5: {
        return m_mesh_vbo5;
        break;
    }
    case 6: {
        return m_mesh_vbo6;
        break;
    }
    case 7: {
        return m_mesh_vbo7;
        break;
    }
    default: {
        return m_mesh_vbo0;
        break;
    }
    }
}

GLuint Realtime::selectVao(int selection) {
    switch (selection) {
    case 0: {
        return m_mesh_vao0;
        break;
    }
    case 1: {
        return m_mesh_vao1;
        break;
    }
    case 2: {
        return m_mesh_vao2;
        break;
    }
    case 3: {
        return m_mesh_vao3;
        break;
    }
    case 4: {
        return m_mesh_vao4;
        break;
    }
    case 5: {
        return m_mesh_vao5;
        break;
    }
    case 6: {
        return m_mesh_vao6;
        break;
    }
    case 7: {
        return m_mesh_vao7;
        break;
    }
    default: {
        return m_mesh_vao0;
        break;
    }
    }
}

// --- student code end ---

void Realtime::initializeGL() {

    m_devicePixelRatio = this->devicePixelRatio();

    m_defaultFBO = 2;
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE); // stop for alpha blending

    // alpha clipping
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here


    // --- student code ---

    // settings changeable
    m_initialized = true;

    // objloader init
    m_objloader = objloader();

    // clear color and load shaders
    glClearColor(0.f, 0.f, 0.f, 1.f);
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    m_texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");

    // loading textures
    glGenTextures(1, &m_texture0);
    glGenTextures(1, &m_texture1);
    glGenTextures(1, &m_texture2);
    glGenTextures(1, &m_texture3);
    glGenTextures(1, &m_texture4);
    glGenTextures(1, &m_texture5);
    glGenTextures(1, &m_texture6);
    glGenTextures(1, &m_texture7);

    glGenTextures(1, &m_normal_texture0);
    glGenTextures(1, &m_normal_texture1);
    glGenTextures(1, &m_normal_texture2);
    glGenTextures(1, &m_normal_texture3);
    glGenTextures(1, &m_normal_texture4);
    glGenTextures(1, &m_normal_texture5);
    glGenTextures(1, &m_normal_texture6);
    glGenTextures(1, &m_normal_texture7);

    glGenTextures(1, &m_opacity_texture0);
    glGenTextures(1, &m_opacity_texture1);
    glGenTextures(1, &m_opacity_texture2);
    glGenTextures(1, &m_opacity_texture3);
    glGenTextures(1, &m_opacity_texture4);
    glGenTextures(1, &m_opacity_texture5);
    glGenTextures(1, &m_opacity_texture6);
    glGenTextures(1, &m_opacity_texture7);

    // leaf

    QString filepath = QString(":/resources/images/leaf_color.png");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());

    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_texture"), 0);
    glUseProgram(0);

    filepath = QString(":/resources/images/leaf_opacity.png");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_opacity_texture0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_opacity_texture"), 1);
    glUseProgram(0);

    filepath = QString(":/resources/images/leaf_normal.png");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_normal_texture0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_normal_texture"), 2);
    glUseProgram(0);

    // bark

    filepath = QString(":/resources/images/bark_color.jpg");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());

    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_texture"), 0);
    glUseProgram(0);

    filepath = QString(":/resources/images/white.png");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_opacity_texture2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_opacity_texture"), 1);
    glUseProgram(0);

    filepath = QString(":/resources/images/bark_normal.jpg");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_normal_texture2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_normal_texture"), 2);
    glUseProgram(0);

    // rocks

    filepath = QString(":/resources/images/rocks_color.jpg");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());

    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_texture"), 0);
    glUseProgram(0);

    filepath = QString(":/resources/images/white.png");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_opacity_texture3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_opacity_texture"), 1);
    glUseProgram(0);

    filepath = QString(":/resources/images/rocks_normal.jpg");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_normal_texture3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_normal_texture"), 2);
    glUseProgram(0);

    // moss

    filepath = QString(":/resources/images/moss_color.jpg");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());

    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_texture"), 0);
    glUseProgram(0);

    filepath = QString(":/resources/images/white.png");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_opacity_texture4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_opacity_texture"), 1);
    glUseProgram(0);

    filepath = QString(":/resources/images/moss_normal.jpg");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_normal_texture4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_normal_texture"), 2);
    glUseProgram(0);

    // grass

    filepath = QString(":/resources/images/grass_color.jpg");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture5);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());

    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_texture"), 0);
    glUseProgram(0);

    filepath = QString(":/resources/images/white.png");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_opacity_texture5);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_opacity_texture"), 1);
    glUseProgram(0);

    filepath = QString(":/resources/images/grass_normal.jpg");
    m_image = QImage(filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_normal_texture5);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "m_normal_texture"), 2);
    glUseProgram(0);

    // set texture shader
    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "sampler"), GL_TEXTURE0);
    glUseProgram(0);

    std::vector<GLfloat> fullscreen_quad_data = {
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f
    };

    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // init FBO
    makeFBO();

    // make default camera
    m_camera.setCamera(
        glm::vec4(3.f, 3.f, 3.f, 1.f),    // pos
        glm::vec4(-3.f, -3.f, -3.f, 0.f), // look
        glm::vec4(0.f, 1.f, 0.f, 0.f),    // up
        0.f,
        0.f,
        0.523599f,
        (float) size().width() / (float) size().height()
    );

    // make default light
    glm::vec4 dir(-3.f, -2.f, -1.f, 1.f);
    glm::vec4 color(1.0, 1.0, 1.0, 1.f);
    m_dirLightDirs[m_numDirLights] = dir;
    m_dirLightColors[m_numDirLights] = color;
    m_numDirLights++;

    // generate vbo's
    glGenBuffers(1, &m_mesh_vbo0);
    glGenBuffers(1, &m_mesh_vbo1);
    glGenBuffers(1, &m_mesh_vbo2);
    glGenBuffers(1, &m_mesh_vbo3);
    glGenBuffers(1, &m_mesh_vbo4);
    glGenBuffers(1, &m_mesh_vbo5);
    glGenBuffers(1, &m_mesh_vbo6);
    glGenBuffers(1, &m_mesh_vbo7);

    // generate vao's
    glGenVertexArrays(1, &m_mesh_vao0);
    glGenVertexArrays(1, &m_mesh_vao1);
    glGenVertexArrays(1, &m_mesh_vao2);
    glGenVertexArrays(1, &m_mesh_vao3);
    glGenVertexArrays(1, &m_mesh_vao4);
    glGenVertexArrays(1, &m_mesh_vao5);
    glGenVertexArrays(1, &m_mesh_vao6);
    glGenVertexArrays(1, &m_mesh_vao7);

    // mesh
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh_vbo0);
    glBufferData(GL_ARRAY_BUFFER, m_meshData0.size() * sizeof(GLfloat), m_meshData0.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_mesh_vao0);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // mesh
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh_vbo1);
    glBufferData(GL_ARRAY_BUFFER, m_meshData1.size() * sizeof(GLfloat), m_meshData1.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_mesh_vao1);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // mesh
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh_vbo2);
    glBufferData(GL_ARRAY_BUFFER, m_meshData2.size() * sizeof(GLfloat), m_meshData2.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_mesh_vao2);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // mesh
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh_vbo3);
    glBufferData(GL_ARRAY_BUFFER, m_meshData3.size() * sizeof(GLfloat), m_meshData3.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_mesh_vao3);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // mesh
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh_vbo4);
    glBufferData(GL_ARRAY_BUFFER, m_meshData4.size() * sizeof(GLfloat), m_meshData4.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_mesh_vao4);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // mesh
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh_vbo5);
    glBufferData(GL_ARRAY_BUFFER, m_meshData5.size() * sizeof(GLfloat), m_meshData5.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_mesh_vao5);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // mesh
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh_vbo6);
    glBufferData(GL_ARRAY_BUFFER, m_meshData6.size() * sizeof(GLfloat), m_meshData6.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_mesh_vao6);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // mesh
    glBindBuffer(GL_ARRAY_BUFFER, m_mesh_vbo7);
    glBufferData(GL_ARRAY_BUFFER, m_meshData7.size() * sizeof(GLfloat), m_meshData7.data(), GL_STATIC_DRAW);
    glBindVertexArray(m_mesh_vao7);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // particle system
    m_generator = *new particlesystem(15);

    // --- student code end ---

}

void Realtime::paintGL() {

    // Students: anything requiring OpenGL calls every frame should be done here

    // --- student code ---

    // camera data
    glm::mat4 m_view = m_camera.getViewMatrix();
    glm::mat4 m_proj = m_camera.getProjectionMatrix();

    // scene data
    float m_ka = 0.5f;
    float m_kd = 0.5f;
    float m_ks = 0.5f;

    // frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_fbo_width, m_fbo_height);

    // clear gl data
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate shader
    glUseProgram(m_shader);

    // pass in camera information
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "m_view"), 1, false, &m_view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "m_proj"), 1, false, &m_proj[0][0]);
    glUniform4fv(glGetUniformLocation(m_shader, "worldSpaceCameraPos"), 1, &m_camera.getPosition()[0]);

    // pass in render data
    glUniform1fv(glGetUniformLocation(m_shader, "m_ka"), 1, &m_ka);
    glUniform1fv(glGetUniformLocation(m_shader, "m_kd"), 1, &m_kd);
    glUniform1fv(glGetUniformLocation(m_shader, "m_ks"), 1, &m_ks);

    // pass in light data
    glUniform1iv(glGetUniformLocation(m_shader, "m_numDirLights"), 1, &m_numDirLights);
    glUniform4fv(glGetUniformLocation(m_shader, "m_dirLightDirs"), 8, &m_dirLightDirs[0][0]);
    glUniform4fv(glGetUniformLocation(m_shader, "m_dirLightColors"), 8, &m_dirLightColors[0][0]);

    glUniform1iv(glGetUniformLocation(m_shader, "m_numPointLights"), 1, &m_numPointLights);
    glUniform4fv(glGetUniformLocation(m_shader, "m_pointLightPos"), 8, &m_pointLightPos[0][0]);
    glUniform4fv(glGetUniformLocation(m_shader, "m_pointLightColors"), 8, &m_pointLightColors[0][0]);
    glUniform3fv(glGetUniformLocation(m_shader, "m_pointLightAttenuation"), 8, &m_pointLightAttenuation[0][0]);

    glUniform1iv(glGetUniformLocation(m_shader, "m_numSpotLights"), 1, &m_numSpotLights);
    glUniform4fv(glGetUniformLocation(m_shader, "m_spotLightPos"), 8, &m_spotLightPos[0][0]);
    glUniform4fv(glGetUniformLocation(m_shader, "m_spotLightDirs"), 8, &m_spotLightDirs[0][0]);
    glUniform4fv(glGetUniformLocation(m_shader, "m_spotLightColors"), 8, &m_spotLightColors[0][0]);
    glUniform3fv(glGetUniformLocation(m_shader, "m_spotLightAttenuation"), 8, &m_spotLightAttenuation[0][0]);
    glUniform1fv(glGetUniformLocation(m_shader, "m_spotLightAngle"), 8, &m_spotLightAngle[0]);
    glUniform1fv(glGetUniformLocation(m_shader, "m_spotLightP"), 8, &m_spotLightP[0]);

    // render mesh
    for (int i = 7; i >= 0; i--) {

        // texture information
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, selectTexture(i));

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, selectOpacityTexture(i));

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, selectNormalTexture(i));

        // calculation information
        glm::mat4 identity(1.f);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "m_model"), 1, false, &identity[0][0]);

        glm::vec4 ambient(0.5f);
        glm::vec4 diffuse(0.5f);
        glm::vec4 specular(0.5f);
        float shininess = 25.f;
        glUniform4fv(glGetUniformLocation(m_shader, "cAmbient"), 1, &ambient[0]);
        glUniform4fv(glGetUniformLocation(m_shader, "cDiffuse"), 1, &diffuse[0]);
        glUniform4fv(glGetUniformLocation(m_shader, "cSpecular"), 1, &specular[0]);
        glUniform1fv(glGetUniformLocation(m_shader, "m_shininess"), 1, &shininess);
        glUniform1i(glGetUniformLocation(m_shader, "m_water"), false);

        glm::vec3 offset(0.f);
        glUniform3fv(glGetUniformLocation(m_shader, "m_offset"), 1, &offset[0]);

        // water overrides
        if (i == 1) {
            ambient  = glm::vec4(0.25f, 0.41f, 0.44f, 1.f);
            diffuse  = glm::vec4(0.25f, 0.41f, 0.44f, 1.f);
            specular = glm::vec4(1.f);
            shininess = 25.f;
            glUniform4fv(glGetUniformLocation(m_shader, "cAmbient"), 1, &ambient[0]);
            glUniform4fv(glGetUniformLocation(m_shader, "cDiffuse"), 1, &diffuse[0]);
            glUniform4fv(glGetUniformLocation(m_shader, "cSpecular"), 1, &specular[0]);
            glUniform1f(glGetUniformLocation(m_shader, "m_shininess"), shininess);

            glUniform1i(glGetUniformLocation(m_shader, "m_water"), true);

            glUniform1i(glGetUniformLocation(m_shader, "m_numWaterPoints"), m_numWaterPoints);
            glUniform4fv(glGetUniformLocation(m_shader, "m_waterPointCenters"), 8, &m_waterPointCenters[0][0]);
            glUniform1fv(glGetUniformLocation(m_shader, "m_waterPointElapsedTimes"), 8, &m_waterPointElapsedTimes[0]);
        }

        // render meshes
        glBindVertexArray(selectVao(i));
        glDrawArrays(GL_TRIANGLES, 0, selectMeshData(i).size() / 3);

    }

    // particle system
    glUniform1i(glGetUniformLocation(m_shader, "m_water"), false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, selectTexture(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, selectOpacityTexture(0));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, selectNormalTexture(0));

    m_generator.draw(m_shader);

    // clean
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    // cleanup frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, m_screen_width * 1.f, m_screen_height * 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // texture shaders
    glUseProgram(m_texture_shader);
    glBindVertexArray(m_fullscreen_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

    auto deltaX = 1.f / m_screen_width;
    auto deltaY = 1.f / m_screen_height;

    glUniform1fv(glGetUniformLocation(m_texture_shader, "deltaX"), 1, &deltaX);
    glUniform1fv(glGetUniformLocation(m_texture_shader, "deltaY"), 1, &deltaY);

    glUniform1i(glGetUniformLocation(m_texture_shader, "filter1"), settings.filter1);
    glUniform1i(glGetUniformLocation(m_texture_shader, "filter2"), settings.filter2);
    glUniform1i(glGetUniformLocation(m_texture_shader, "filter3"), settings.filter3);
    glUniform1i(glGetUniformLocation(m_texture_shader, "filter4"), settings.filter4);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    // --- student code end ---

}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    // --- student code ---

    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    makeFBO();

    // --- student code end ---

}

void Realtime::sceneChanged(int selection) {

    // --- student code ---

    std::cout << "scene file loaded to slot: " << selection << std::endl;

    // obj loading
    switch (selection) {
        case 0: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData0);
            break;
        }
        case 1: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData1);
            break;
        }
        case 2: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData2);
            break;
        }
        case 3: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData3);
            break;
        }
        case 4: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData4);
            break;
        }
        case 5: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData5);
            break;
        }
        case 6: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData6);
            break;
        }
        case 7: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData7);
            break;
        }
        default: {
            m_objloader.parse(settings.sceneFilePath.c_str(), m_meshData0);
            break;
        }
    }

    // update mesh data
    glBindBuffer(GL_ARRAY_BUFFER, selectVbo(selection));
    glBufferData(GL_ARRAY_BUFFER, selectMeshData(selection).size() * sizeof(GLfloat), selectMeshData(selection).data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // --- student code end ---

    update(); // asks for a PaintGL() call to occur

}

void Realtime::settingsChanged() {

    // --- student code ---

    // check gl initialization
    if (!m_initialized) return;

    // update if the view planes have changed
    if (m_prevParam2 != settings.nearPlane || m_prevParam3 != settings.farPlane) m_camera.update();

    // update params
    m_prevParam2 = settings.nearPlane;
    m_prevParam3 = settings.farPlane;

    // --- student code end ---

    update(); // asks for a PaintGL() call to occur

}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate

        // --- student code ---

        // rotation
        if (deltaX != 0 || deltaY != 0) {
            m_camera.updateLook(deltaX, deltaY);
        }

        // --- student code end ---

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::pushRipple(glm::vec4 waterCenterPoint) {

    // increment num water points
    m_numWaterPoints += m_numWaterPoints >= 8 ? 0 : 1;

    // make room for new water point
    for (int i = m_numWaterPoints; i > 0; i--) {
        if (i == 8) continue;
        m_waterPointElapsedTimes[i] = m_waterPointElapsedTimes[i - 1];
        m_waterPointCenters[i] = m_waterPointCenters[i - 1];
    }

    // add new water point
    m_waterPointElapsedTimes[0] = 0.f;
    m_waterPointCenters[0] = waterCenterPoint;

}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around

    // --- student code ---

    // movement
    glm::vec3 deltaPos(0.f);

    if (m_keyMap[Qt::Key_W]) {
        deltaPos = deltaPos + m_camera.getLook() * 5.f * deltaTime;
    }
    if (m_keyMap[Qt::Key_A]) {
        deltaPos = deltaPos - glm::cross(m_camera.getLook(), m_camera.getUp()) * 5.f * deltaTime;
    }
    if (m_keyMap[Qt::Key_S]) {
        deltaPos = deltaPos - m_camera.getLook() * 5.f * deltaTime;
    }
    if (m_keyMap[Qt::Key_D]) {
        deltaPos = deltaPos + glm::cross(m_camera.getLook(), m_camera.getUp()) * 5.f * deltaTime;
    }

    if (m_keyMap[Qt::Key_Space]) {
        deltaPos = deltaPos + glm::vec3(0.f, 1.f, 0.f) * 5.f * deltaTime;
    }
    if (m_keyMap[Qt::Key_Control]) {
        deltaPos = deltaPos - glm::vec3(0.f, 1.f, 0.f) * 5.f * deltaTime;
    }

    if (deltaPos != glm::vec3(0.f)) {
        // update the camera position
        m_camera.updatePosition(deltaPos);
    }

    // particle system
    std::vector<glm::vec3> positions = m_generator.updateParticles(deltaTime);

    // particles to ripple system
    for (auto pos : positions) {
        pushRipple(glm::vec4(pos[0], 0.f, pos[2], 1.f));
    }

    // update water ripples elapsed times
    for (int i = 0; i < m_numWaterPoints; i++) {
        m_waterPointElapsedTimes[i] += deltaTime;
    }

    // --- student code end ---

    update(); // asks for a PaintGL() call to occur

}

// DO NOT EDIT
void Realtime::saveViewportImage(std::string filePath) {
    // Make sure we have the right context and everything has been drawn
    makeCurrent();

    int fixedWidth = 1024;
    int fixedHeight = 768;

    // Create Frame Buffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a color attachment texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fixedWidth, fixedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Optional: Create a depth buffer if your rendering uses depth testing
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fixedWidth, fixedHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Render to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, fixedWidth, fixedHeight);

    // Clear and render your scene here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintGL();

    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(fixedWidth * fixedHeight * 3);
    glReadPixels(0, 0, fixedWidth, fixedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the framebuffer to return to default rendering to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Convert to QImage
    QImage image(pixels.data(), fixedWidth, fixedHeight, QImage::Format_RGB888);
    QImage flippedImage = image.mirrored(); // Flip the image vertically

    // Save to file using Qt
    QString qFilePath = QString::fromStdString(filePath);
    if (!flippedImage.save(qFilePath)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    }

    // Clean up
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}
