#include "glrenderer.h"

#include <QCoreApplication>
#include "src/shaderloader.h"

#include <QMouseEvent>
#include "glm/gtx/transform.hpp"
#include <glm/gtc/type_ptr.hpp>

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
    makeCurrent();
    doneCurrent();

    //water related destroy
    glDeleteVertexArrays(1, &m_water_vao);
    glDeleteBuffers(1, &m_water_vbo);

    //clean up FBOs
    glDeleteFramebuffers(1, &reflectionFBO);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);

    glDeleteFramebuffers(1, &refractionFBO);
    glDeleteTextures(1, &refractionTexture);
    glDeleteTextures(1, &refractionDepthTexture);
}

// ================== Helper Functions
void GLRenderer::checkFBOStatus() {//for debugging purpose
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    }
}

GLuint GLRenderer::createTextureAttachment(int width, int height) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    return textureID;
}

GLuint GLRenderer::createDepthTextureAttachment(int width, int height) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
    return textureID;
}

GLuint GLRenderer::createDepthBufferAttachment(int width, int height) {
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    return depthBuffer;
}

void GLRenderer::initializeReflectionFBO() {
    glGenFramebuffers(1, &reflectionFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO); //reflection FBO is bounded

    reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    std::cout<<"REFLECTION!!"<<std::endl;
    checkFBOStatus(); //this still outputs framebuffer not complete

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void GLRenderer::initializeRefractionFBO() {
    glGenFramebuffers(1, &refractionFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFBO); //refraction FBO is bounded


    refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    std::cout<<"REFRACTION!!"<<std::endl;
    checkFBOStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderer::bindReflectionFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
    glViewport(0, 0, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void GLRenderer::bindRefractionFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFBO);
    glViewport(0, 0, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void GLRenderer::unbindCurrentFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, this->width(), this->height());
}


// ================== Rendering Pipeline
// Water quad vertex positions
const GLfloat waterVertices[] = {
    -1.0f, -1.0f,  // Bottom Left
    1.0f, -1.0f,  // Bottom Right
    1.0f,  1.0f,  // Top Right
    -1.0f,  1.0f   // Top Left
};

void GLRenderer::initializeGL() {
    // ... existing initialization code ...
    // Initialize GL extension wrangler
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) fprintf(stderr, "Error while initializing GLEW: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Successfully initialized GLEW %s\n", glewGetString(GLEW_VERSION));

    // Set clear color to black
    glClearColor(0,0,0,1);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    m_devicePixelRatio = this->devicePixelRatio();
    m_shader = ShaderLoader::createShaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");

    /**
     * Here begins loading rendering data
    **/
    // Generate and bind VBO for the tree
    glGenBuffers(1, &m_tree_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_tree_vbo);

    //Load OBJ file
    std::vector<float> data;
    std::vector<std::string> line;
        bool res = objloader.loadOBJ("scenefiles/cube.obj",
                                     data,
                                     line);

        if (!res) {
            std::cout << "error parsing obj file" << std::endl;
            return;
        }

    m_treeData = data;

    // Assuming m_treeData is already populated with tree vertex data
    glBufferData(GL_ARRAY_BUFFER, m_treeData.size() * sizeof(GLfloat), m_treeData.data(), GL_STATIC_DRAW);

    // Generate and bind VAO for the tree
    glGenVertexArrays(1, &m_tree_vao);
    glBindVertexArray(m_tree_vao);

    // Enable and define attribute arrays for the tree VAO
    glEnableVertexAttribArray(0); // For position
    glEnableVertexAttribArray(1); // For UV
    glEnableVertexAttribArray(2); // For normals
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(5 * sizeof(GLfloat)));

    // Unbind the tree VAO
    glBindVertexArray(0);

    // Generate and bind VBO for water
    glGenBuffers(1, &m_water_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_water_vbo);
    // Assuming waterVertices is already defined
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), waterVertices, GL_STATIC_DRAW);

    // Generate and bind VAO for water
    glGenVertexArrays(1, &m_water_vao);
    glBindVertexArray(m_water_vao);

    // Enable and define attribute arrays for the water VAO
    glEnableVertexAttribArray(0); // For position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), reinterpret_cast<void*>(0));

    // Unbind the water VAO
    glBindVertexArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /**
     * Load data end, deal with FBO and texture shader now
     **/
    //    // Initialize reflection and refraction framebuffers
    //    std::cout<<"Start Initialization"<<std::endl;
    initializeReflectionFBO();
    initializeRefractionFBO();

    m_texture_shader = ShaderLoader::createShaderProgram("resources/shaders/texture.vert", "resources/shaders/texture.frag");

    std::vector<GLfloat> fullscreen_quad_data =
        { //     POSITIONS   // // UV Coords //
            -1.0f,  1.0f, 0.0f,  0.0f,  1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
            1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
            1.0f,  1.0f, 0.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, 0.0f,  0.0f,  1.0f,
            1.0f, -1.0f, 0.0f,  1.0f,  0.0f
        };

    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "tex"), 0);
    glUniform1f(glGetUniformLocation(m_texture_shader, "deltaU"), 1.0f/(size().width() * m_devicePixelRatio));
    glUniform1f(glGetUniformLocation(m_texture_shader, "deltaV"), 1.0f/(size().height()  * m_devicePixelRatio));

    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1); // uv coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),  reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(0);
}



void GLRenderer::paintGL()
{
    // Clear screen color and depth before painting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //activate the shader program by calling glUseProgram with `m_shader`
    glUseProgram(m_shader);

    //pass in m_model as a uniform into the shader program
    GLint output = glGetUniformLocation(m_shader, "model");
    if (output == -1) {
        std::cout << "no location for model" << std::endl;
        return;
    }
    glUniformMatrix4fv(output, 1, GL_FALSE, &m_model[0][0]);

    //pass in m_view and m_proj
    GLint view_loc = glGetUniformLocation(m_shader, "view");
    GLint proj_loc = glGetUniformLocation(m_shader, "proj");

    if (view_loc == -1 || proj_loc == -1) {
        std::cout << "view and/or projection matrix not found" << std::endl;
        return;
    }

    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &m_view[0][0]);
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

    // pass shininess, m_ks, and world-space camera position
    GLint ks_loc = glGetUniformLocation(m_shader, "m_ks");
    GLint shiny_loc = glGetUniformLocation(m_shader, "shininess");
    GLint cam_loc = glGetUniformLocation(m_shader, "cam_pos");

    if (ks_loc == -1 || shiny_loc == -1 || cam_loc == -1) {
        std::cout << "specular term not found" << std::endl;
        return;
    }

    glUniform1f(ks_loc, m_ks);
    glUniform1f(shiny_loc, m_shininess);

    glm::vec4 cameraPos = inverse(m_view) * glm::vec4(0.0, 0.0, 0.0, 1.0);
    glUniform4fv(cam_loc, 1, &cameraPos[0]);


    //reflection pass
//    bindReflectionFBO();
//    unbindCurrentFBO();

//    //refraction pass
//    bindRefractionFBO();
//    unbindCurrentFBO();

    //Render Tree
    glUniform1i(glGetUniformLocation(m_shader, "isWater"), 1); //render water off
    glBindVertexArray(m_tree_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_treeData.size() / 3);
    // Unbind Vertex Array
    glBindVertexArray(0);

    // Render Water
    glUniform1i(glGetUniformLocation(m_shader, "isWater"), 1); //render water on


    glBindVertexArray(m_water_vao);
    // Set up uniforms specific to the water (model matrix for water, etc.)
    m_waterModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    GLint waterModelLoc = glGetUniformLocation(m_shader, "waterModel");
    glUniformMatrix4fv(waterModelLoc, 1, GL_FALSE, glm::value_ptr(m_waterModelMatrix));
    // Bind reflection and refraction textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, refractionTexture);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Assuming water quad has 4 vertices
    glBindVertexArray(0);

    //deactivate the shader program by passing 0 into glUseProgram
    glUseProgram(0);
}

// ================== Other codes

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
    glm::vec3 eye = glm::vec3(2,0,0);
    eye = glm::vec3(rot * glm::vec4(eye,1));
    rot = glm::rotate(glm::radians(-10 * m_angleY),glm::cross(glm::vec3(0,0,1),eye));
    eye = glm::vec3(rot * glm::vec4(eye,1));

    eye = eye * m_zoom;

    m_view = glm::lookAt(eye,glm::vec3(0,0,0),glm::vec3(0,0,1));

    m_proj = glm::perspective(glm::radians(45.0),1.0 * width() / height(),0.01,100.0);

    update();
}
