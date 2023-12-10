#include "glrenderer.h"

#include <QCoreApplication>
#include "src/shaderloader.h"

#include <QMouseEvent>
#include "glm/gtx/transform.hpp"

#include "src/objloader.h"

objloader objloader;

GLRenderer::GLRenderer(QWidget *parent)
    : QOpenGLWidget(parent),
      m_lightPos(10,0,0,1),
      m_ka(0.1),
      m_kd(0.8),
      m_ks(1),
      m_shininess(15),
      m_angleX(6),
      m_angleY(0),
      m_zoom(2)
{
    rebuildMatrices();
}

GLRenderer::~GLRenderer()
{
    killTimer(m_timer);
    makeCurrent();
    doneCurrent();
}

// ================== Helper Functions

glm::vec4 sphericalToCartesian(float phi, float theta)
{
    return glm::vec4(glm::cos(theta) * glm::sin(phi),
                     glm::sin(theta) * glm::sin(phi),
                     glm::cos(phi),1);
}

void pushVec3(glm::vec4 vec, std::vector<float>* data)
{
    data->push_back(vec.x);
    data->push_back(vec.y);
    data->push_back(vec.z);
}

std::vector<float> generateSphereData(int phiTesselations, int thetaTesselations)
{
    std::vector<float> data;

    data.clear();
    data.reserve(phiTesselations * thetaTesselations * 6 * 3);

    for(int iTheta = 0; iTheta < thetaTesselations; iTheta++) {
        for(int iPhi = 0; iPhi < phiTesselations; iPhi++) {
            float phi1 = 1.0  * iPhi / phiTesselations * glm::pi<float>();
            float phi2 = 1.0  * (iPhi + 1) / phiTesselations * glm::pi<float>();

            float the1 = 1.0 * iTheta / thetaTesselations * 2 * glm::pi<float>();
            float the2 = 1.0 * (iTheta + 1) / thetaTesselations * 2 * glm::pi<float>();

            glm::vec4 p1 = sphericalToCartesian(phi1,the1);
            glm::vec4 p2 = sphericalToCartesian(phi2,the1);
            glm::vec4 p3 = sphericalToCartesian(phi2,the2);
            glm::vec4 p4 = sphericalToCartesian(phi1,the2);

            pushVec3(p1,&data);
            pushVec3(p2,&data);
            pushVec3(p3,&data);

            pushVec3(p1,&data);
            pushVec3(p3,&data);
            pushVec3(p4,&data);
        }
    }

    return data;
}

// ================== Students, You'll Be Working In These Files

void GLRenderer::initializeGL()
{
    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initialize GL extension wrangler
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) fprintf(stderr, "Error while initializing GLEW: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Successfully initialized GLEW %s\n", glewGetString(GLEW_VERSION));

    // Set clear color to black
    glClearColor(0,0,0,1);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Task 1: call ShaderLoader::createShaderProgram with the paths to the vertex
    //         and fragment shaders. Then, store its return value in `m_shader`
    std::cout << "here" << std::endl;
    m_shader = ShaderLoader::createShaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");
    std::cout << "here1" << std::endl;

    // Generate and bind VBO
    glGenBuffers(1, &m_sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphere_vbo);
    // Generate sphere data
//    m_sphereData = generateSphereData(10,20);
    std::vector<float> data;
    std::vector<std::string> line;
    bool res = objloader.loadOBJ("scenefiles/tess_plane.obj",
                                 data,
                                 line);

    if (!res) {
        std::cout << "error parsing obj file" << std::endl;
        return;
    }
    std::cout << "data read successfully" << std::endl;
    m_sphereData = data;
    // Send data to VBO
    glBufferData(GL_ARRAY_BUFFER,m_sphereData.size() * sizeof(GLfloat),m_sphereData.data(), GL_STATIC_DRAW);
    // Generate, and bind vao
    glGenVertexArrays(1, &m_sphere_vao);
    glBindVertexArray(m_sphere_vao);

    // Enable and define attribute 0 to store vertex positions
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),reinterpret_cast<void *>(0));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat), reinterpret_cast<void *>(5 * sizeof(GLfloat)));

    // Clean-up bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void GLRenderer::paintGL()
{
    // Clear screen color and depth before painting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Bind Sphere Vertex Data
    glBindVertexArray(m_sphere_vao);

    // Task 2: activate the shader program by calling glUseProgram with `m_shader`
    glUseProgram(m_shader);

    // Task 6: pass in m_model as a uniform into the shader program
    GLint output = glGetUniformLocation(m_shader, "model");
    if (output == -1) {
        std::cout << "no location for model" << std::endl;
        return;
    }
    glUniformMatrix4fv(output, 1, GL_FALSE, &m_model[0][0]);

    GLint time_loc = glGetUniformLocation(m_shader, "time");
    glUniform1f(time_loc, m_time);

    // Task 7: pass in m_view and m_proj
    GLint view_loc = glGetUniformLocation(m_shader, "view");
    GLint proj_loc = glGetUniformLocation(m_shader, "proj");

    if (view_loc == -1 || proj_loc == -1) {
        std::cout << "view and/or projection matrix not found" << std::endl;
        return;
    }

    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &m_view[0][0]);
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &m_proj[0][0]);

    // Task 12: pass m_ka into the fragment shader as a uniform
    GLint ka_loc = glGetUniformLocation(m_shader, "m_ka");
    if (ka_loc == -1) {
        std::cout << "ambient term not found" << std::endl;
        return;
    }
    glUniform1f(ka_loc, m_ka);

    // Task 13: pass light position and m_kd into the fragment shader as a uniform
    GLint kd_loc = glGetUniformLocation(m_shader, "m_kd");
    GLint light_loc = glGetUniformLocation(m_shader, "light_pos");

    if (kd_loc == -1 || light_loc == -1) {
        std::cout << "diffuse term not found" << std::endl;
        return;
    }

    glUniform1f(kd_loc, m_kd);
    glUniform4fv(light_loc, 1, &m_lightPos[0]);

    // Task 14: pass shininess, m_ks, and world-space camera position
    GLint ks_loc = glGetUniformLocation(m_shader, "m_ks");
    GLint shiny_loc = glGetUniformLocation(m_shader, "shininess");
    GLint cam_loc = glGetUniformLocation(m_shader, "cam_pos");

    if (ks_loc == -1 || shiny_loc == -1 || cam_loc == -1) {
        std::cout << "specular term not found" << std::endl;
        return;
    }

    glUniform1f(ks_loc, m_ks);
    glUniform1f(shiny_loc, m_shininess);

    glm::vec4 cameraPos = inverse(m_view) * glm::vec4(3.0, 10.0, 4.0, 1.0);
    glUniform4fv(cam_loc, 1, &cameraPos[0]);

    // Draw Command
    glDrawArrays(GL_TRIANGLES, 0, m_sphereData.size() / 3);
    // Unbind Vertex Array
    glBindVertexArray(0);

    // Task 3: deactivate the shader program by passing 0 into glUseProgram
    glUseProgram(0);
}

// ================== Other stencil code

void GLRenderer::resizeGL(int w, int h)
{
    m_proj = glm::perspective(glm::radians(45.0),1.0 * w / h,0.01,100.0);
}

void GLRenderer::mousePressEvent(QMouseEvent *event) {
    // Set initial mouse position
    m_prevMousePos = event->pos();
}

void GLRenderer::mouseMoveEvent(QMouseEvent *event) {
    // Update angle member variables based on event parameters
    m_angleX += 10 * (event->position().x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 10 * (event->position().y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void GLRenderer::wheelEvent(QWheelEvent *event) {
    // Update zoom based on event parameter
    m_zoom -= event->angleDelta().y() / 100.f;
    rebuildMatrices();
}

void GLRenderer::rebuildMatrices() {
    // Update view matrix by rotating eye vector based on x and y angles
    m_view = glm::mat4(1);
    glm::mat4 rot = glm::rotate(glm::radians(-10 * m_angleX),glm::vec3(0,0,1));
    glm::vec3 eye = glm::vec3(3,10,4);
    eye = glm::vec3(rot * glm::vec4(eye,1));

    rot = glm::rotate(glm::radians(-10 * m_angleY),glm::cross(glm::vec3(0,0,1),eye));
    eye = glm::vec3(rot * glm::vec4(eye,1));

    eye = eye * m_zoom;

    m_view = glm::lookAt(eye,glm::vec3(0,0,0),glm::vec3(0,1,0));

    m_proj = glm::perspective(glm::radians(45.0),1.0 * width() / height(),0.01,100.0);

    update();
}

void GLRenderer::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
//    m_elapsedTimer.restart();

    m_time = deltaTime;

    update(); // asks for a PaintGL() call to occur
}
