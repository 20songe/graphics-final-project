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

void pushVec2(glm::vec2 vec, std::vector<float>* data) {
    data->push_back(vec.x);
    data->push_back(vec.y);
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

void GLRenderer::makeFBO(){
    // Generate and bind an empty texture, set its min/mag filter interpolation, then unbind
    glGenTextures(1, &m_fbo_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Generate and bind a renderbuffer of the right size, set its format, then unbind
    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Generate and bind an FBO
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Add our texture as a color attachment, and our renderbuffer as a depth+stencil attachment, to our FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    // Unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);

}



// ================== Students, You'll Be Working In These Files

void GLRenderer::initializeGL(){
    m_defaultFBO = 2;
    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    m_devicePixelRatio = this->devicePixelRatio();

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    // Initialize GL extension wrangler
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) fprintf(stderr, "Error while initializing GLEW: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Successfully initialized GLEW %s\n", glewGetString(GLEW_VERSION));

    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Set clear color to black
    glClearColor(0, 0, 0,1);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    //call ShaderLoader::createShaderProgram with the paths to the vertex
    //and fragment shaders. Then, store its return value in `m_shader`
    std::cout << "here" << std::endl;
    m_shader = ShaderLoader::createShaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");
    m_texture_shader = ShaderLoader::createShaderProgram("resources/shaders/reflection_texture.vert", "resources/shaders/reflection_texture.frag");
    std::cout << "here1" << std::endl;


    /**
     * Loading Image, can be refactored
     **/
    //load the dudv map image
    QString dudv_filepath = QString("resources/waterDUDV.png");
//    QString normal_map_filepath = QString("resources/normalMap.png");
    QString normal_map_filepath = QString("resources/waterNormal_3.png");

    //obtain image from filepath
    m_image1.load(dudv_filepath);

    if(m_image1.isNull()){
         qDebug() << "Failed to load the image:" << dudv_filepath;
    } else {
         //Format image to fit OpenGL
         m_image1 = m_image1.convertToFormat(QImage::Format_RGBA8888).mirrored(false, true);

         //Generate dudv texture
         glGenTextures(1, &m_dudv_texture);

         //Bind dudv texture
         glBindTexture(GL_TEXTURE_2D, m_dudv_texture);

         //Load image into dudv texture
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image1.width(), m_image1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image1.bits());

         //Set min and mag filters' interpolation mode to linear
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         //
         //Unbind dudv texture
         glBindTexture(GL_TEXTURE_2D, 0);
    }


    //obtain image from filepath
    m_image2.load(normal_map_filepath);

    if(m_image2.isNull()){
         qDebug() << "Failed to load the image:" << normal_map_filepath;
    } else {
         //Format image to fit OpenGL
         m_image2 = m_image2.convertToFormat(QImage::Format_RGBA8888).mirrored(false, true);

         //Generate dudv texture
         glGenTextures(1, &m_normal_texture);

         //Bind dudv texture
         glBindTexture(GL_TEXTURE_2D, m_normal_texture);

         //Load image into normal texture
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image2.width(), m_image2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image2.bits());

         //Set min and mag filters' interpolation mode to linear
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         //
         //Unbind dudv texture
         glBindTexture(GL_TEXTURE_2D, 0);
    }

    /**
     * Load Image ends
     */

    glUseProgram(m_texture_shader);

    GLuint sampler_loc = glGetUniformLocation(m_texture_shader, "texSampler");
    glUniform1i(sampler_loc, 0);

    GLint dudvMap_loc = glGetUniformLocation(m_texture_shader, "dudvMap");
    glUniform1i(dudvMap_loc, 1); // Set the dudvMap uniform to use texture slot 1

    GLint normalMap_loc = glGetUniformLocation(m_texture_shader, "normalMap");
    glUniform1i(normalMap_loc, 2); // Set the dudvMap uniform to use texture slot 2
    Debug::glErrorCheck();

    glUseProgram(0);

    // Generate and bind VBO
    glGenBuffers(1, &m_obj_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_obj_vbo);
    // Generate data
    std::vector<float> data;
    std::vector<std::string> line;
    bool res = objloader.loadOBJ("scenefiles/tree.obj",
                                 data,
                                 line);

    if (!res) {
        std::cout << "error parsing obj file" << std::endl;
        return;
    }
    std::cout << "data read successfully" << std::endl;
    m_objData = data;
    // Send data to VBO
    glBufferData(GL_ARRAY_BUFFER,m_objData.size() * sizeof(GLfloat),m_objData.data(), GL_STATIC_DRAW);
    // Generate, and bind vao
    glGenVertexArrays(1, &m_obj_vao);
    glBindVertexArray(m_obj_vao);

    // Enable and define attribute 0 to store vertex positions
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,9 * sizeof(GLfloat),reinterpret_cast<void *>(0));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,9 * sizeof(GLfloat),reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,9 * sizeof(GLfloat), reinterpret_cast<void *>(5 * sizeof(GLfloat)));
    glVertexAttribPointer(3,1,GL_FLOAT,GL_FALSE,9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat))); //here you define the obj attribute
    // Clean-up bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    // multiply corners of plane by projection matrix
    std::vector<GLfloat> fullscreen_quad_data =
        { //     POSITIONS    //
            -1.f,  1.f, 0.0f,
            0.0f, 1.0f,
            -1.f, -1.f, 0.0f,
            0.0f, 0.0f,
            1.f, -1.f, 0.0f,
            1.0f, 0.0f,
            1.f,  1.f, 0.0f,
            1.0f, 1.0f,
            -1.f,  1.f, 0.0f,
            0.0f, 1.0f,
            1.f, -1.f, 0.0f,
            1.0f, 0.0f
        };

    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    //modify the code below to add a second attribute to the vertex attribute array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),  reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeFBO();
}

void GLRenderer::paintGL()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    //activate the shader program by calling glUseProgram with `m_shader`
    glUseProgram(m_shader);

    // Clear screen color and depth before painting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Bind Sphere Vertex Data
    glBindVertexArray(m_obj_vao);

    //pass in m_model as a uniform into the shader program
    GLint output = glGetUniformLocation(m_shader, "model");
    if (output == -1) {
        std::cout << "no location for model" << std::endl;
        return;
    }
    glUniformMatrix4fv(output, 1, GL_FALSE, &m_model[0][0]);

    GLint time_loc = glGetUniformLocation(m_shader, "time");
    glUniform1f(time_loc, m_time);

    //pass in m_view and m_proj
    GLint view_loc = glGetUniformLocation(m_shader, "view");
    GLint proj_loc = glGetUniformLocation(m_shader, "proj");

    if (view_loc == -1 || proj_loc == -1) {
        std::cout << "view and/or projection matrix not found" << std::endl;
        return;
    }

    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &m_reflect[0][0]);
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &m_proj[0][0]);

    //pass m_ka into the fragment shader as a uniform
    GLint ka_loc = glGetUniformLocation(m_shader, "m_ka");
    if (ka_loc == -1) {
        std::cout << "ambient term not found" << std::endl;
        return;
    }
    glUniform1f(ka_loc, m_ka);

    //pass light position and m_kd into the fragment shader as a uniform
    GLint kd_loc = glGetUniformLocation(m_shader, "m_kd");
    GLint light_loc = glGetUniformLocation(m_shader, "light_pos");

    if (kd_loc == -1 || light_loc == -1) {
        std::cout << "diffuse term not found" << std::endl;
        return;
    }

    glUniform1f(kd_loc, m_kd);
    glUniform4fv(light_loc, 1, &m_lightPos[0]);

    //pass shininess, m_ks, and world-space camera position
    GLint ks_loc = glGetUniformLocation(m_shader, "m_ks");
    GLint shiny_loc = glGetUniformLocation(m_shader, "shininess");
    GLint cam_loc = glGetUniformLocation(m_shader, "cam_pos");

    if (ks_loc == -1 || shiny_loc == -1 || cam_loc == -1) {
        std::cout << "specular term not found" << std::endl;
        return;
    }

    glUniform1f(ks_loc, m_ks);
    glUniform1f(shiny_loc, m_shininess);

    glm::vec4 cameraPos = m_inv_view * glm::vec4(3.0, 10.0, 4.0, 1.0);
    glUniform4fv(cam_loc, 1, &cameraPos[0]);

    // Draw Command
    glDrawArrays(GL_TRIANGLES, 0, m_objData.size() / 9);
    // Unbind Vertex Array
    glBindVertexArray(0);

    //deactivate the shader program by passing 0 into glUseProgram
    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, m_screen_width, m_screen_height);

    // clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    paintTexture(m_fbo_texture);
}

void GLRenderer::paintTexture(GLuint texture){
    glUseProgram(m_texture_shader);
    //Set your bool uniform on whether or not to filter the texture drawn

    GLuint height_loc = glGetUniformLocation(m_texture_shader, "height");
    GLuint width_loc = glGetUniformLocation(m_texture_shader, "width");

    glUniform1f(height_loc, float(m_fbo_height));
    glUniform1f(width_loc, float(m_fbo_width));

    GLuint time_loc = glGetUniformLocation(m_texture_shader, "time");
    glUniform1f(time_loc, m_time);

    GLuint center_loc = glGetUniformLocation(m_texture_shader, "center");
    glUniform4fv(center_loc, 1, &center[0]);

    GLuint view_loc = glGetUniformLocation(m_texture_shader, "view");
    GLuint proj_loc = glGetUniformLocation(m_texture_shader, "proj");

    if (view_loc == -1 || proj_loc == -1) {
        std::cout << "view and/or projection matrix not found" << std::endl;
        return;
    }

    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &m_view[0][0]);
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &m_proj[0][0]);

    // Task 12: pass m_ka into the fragment shader as a uniform
    GLuint ka_loc = glGetUniformLocation(m_texture_shader, "m_ka");
    if (ka_loc == -1) {
        std::cout << "ambient term not found" << std::endl;
        return;
    }
    glUniform1f(ka_loc, m_ka);

    //pass light position and m_kd into the fragment shader as a uniform
    GLuint kd_loc = glGetUniformLocation(m_texture_shader, "m_kd");
    GLuint light_loc = glGetUniformLocation(m_texture_shader, "light_pos");

    if (kd_loc == -1 || light_loc == -1) {
        std::cout << "diffuse term not found" << std::endl;
        return;
    }

    glUniform1f(kd_loc, m_kd);
    glUniform4fv(light_loc, 1, &m_lightPos[0]);

    // Task 14: pass shininess, m_ks, and world-space camera position
    GLuint ks_loc = glGetUniformLocation(m_texture_shader, "m_ks");
    GLuint shiny_loc = glGetUniformLocation(m_texture_shader, "shininess");
    GLuint cam_loc = glGetUniformLocation(m_texture_shader, "cam_pos");

    if (ks_loc == -1 || shiny_loc == -1 || cam_loc == -1) {
        std::cout << "specular term not found" << std::endl;
        return;
    }

    glUniform1f(ks_loc, m_ks);
    glUniform1f(shiny_loc, m_shininess);

    glm::vec4 cameraPos = m_inv_view * glm::vec4(3.0, 10.0, 4.0, 1.0);
    glUniform4fv(cam_loc, 1, &cameraPos[0]);

    glBindVertexArray(m_obj_vao);

    //Bind "texture" to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    //bind "dudv_texture" to slot 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_dudv_texture);

    //bind "normal_texture" to slot 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_normal_texture);

    glDrawArrays(GL_TRIANGLES, 0, m_objData.size() / 9);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

// ================== Other stencil code

void GLRenderer::resizeGL(int w, int h)
{
    m_proj = glm::perspective(glm::radians(45.0),1.0 * w / h,0.01,100.0);
    //change the FBO width and height
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    makeFBO();
}

void GLRenderer::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_elapsedTimer.restart();
        center += glm::vec4(0.5, 0.0, 0.0, 0.0);
    }

    m_prevMousePos = event->pos();
    should_ripple = !should_ripple;
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
    m_inv_view = inverse(m_view);
    m_reflect = glm::mat4(1,0,0,0,
                            0,-1,0,0,
                            0,0,1,0,
                            0,0,0,1) * m_view;

    m_proj = glm::perspective(glm::radians(45.0),1.0 * width() / height(),0.01,100.0);

    update();
}

void GLRenderer::timerEvent(QTimerEvent *event) {
    if (should_ripple) {
        int elapsedms   = m_elapsedTimer.elapsed();
        float deltaTime = elapsedms * 0.001f;

        m_time = deltaTime / 5;
    }
    else {
        m_time = -1;
    }


    update(); // asks for a PaintGL() call to occur
}
