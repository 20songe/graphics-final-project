#include "particlesystem.h"
#include <iostream>

particlesystem::particlesystem() {} // empty constructor

particlesystem::particlesystem(unsigned int n): // init constructor
    n(n)
{
    init();
}

glm::mat4 toMat4(glm::mat3 &mat3) {
    return glm::mat4(
        mat3[0][0], mat3[0][1], mat3[0][2], 0.f,
        mat3[1][0], mat3[1][1], mat3[1][2], 0.f,
        mat3[2][0], mat3[2][1], mat3[2][2], 0.f,
        0.f, 0.f, 0.f, 1.f
    );
}

glm::mat3 rotationParticle(float angle, glm::vec3 axis) {

    return glm::mat3{
        glm::cos(angle) + axis[0] * axis[0] * (1 - glm::cos(angle)),
        axis[0] * axis[1] * (1 - glm::cos(angle)) + axis[2] * glm::sin(angle),
        -(axis[0] * axis[2] * (1 - glm::cos(angle)) + axis[1] * glm::sin(angle)),

        axis[0] * axis[1] * (1 - glm::cos(angle)) - axis[2] * glm::sin(angle),
        glm::cos(angle) + axis[1] * axis[1] * (1 - glm::cos(angle)),
        axis[1] * axis[2] * (1 - glm::cos(angle)) + axis[0] * glm::sin(angle),

        axis[0] * axis[2] * (1 - glm::cos(angle)) + axis[1] * glm::sin(angle),
        axis[1] * axis[2] * (1 - glm::cos(angle)) - axis[0] * glm::sin(angle),
        glm::cos(angle) + axis[2] * axis[2] * (1 - glm::cos(angle))
    };

}

void particlesystem::init() {

    // particle quad
    float size = 0.125f;
    std::vector<GLfloat> particle_quad = { // vertex, normals, uvs, material
        size, 0.f,  size,  0.f, 1.f, 0.f,  1.f, 0.f,  1.f,
       -size, 0.f, -size,  0.f, 1.f, 0.f,  0.f, 1.f,  1.f,
       -size, 0.f,  size,  0.f, 1.f, 0.f,  0.f, 0.f,  1.f,

        size, 0.f,  size,  0.f, 1.f, 0.f,  1.f, 0.f,  1.f,
        size, 0.f, -size,  0.f, 1.f, 0.f,  1.f, 1.f,  1.f,
       -size, 0.f, -size,  0.f, 1.f, 0.f,  0.f, 1.f,  1.f
    };

    // generate vbo and vao
    glGenBuffers(1, &m_particle_vbo);
    glGenVertexArrays(1, &m_particle_vao);

    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_particle_vbo);
    glBufferData(GL_ARRAY_BUFFER, particle_quad.size() * sizeof(GLfloat), particle_quad.data(), GL_STATIC_DRAW);

    // set mesh attributes
    glBindVertexArray(m_particle_vao);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // material index
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < n; ++i)
        particles.push_back(Particle());
    for (auto& particle : particles)
        resetParticle(particle);

}

// reset particles for reuse
void particlesystem::resetParticle(Particle &particle) {

    float radius = 5.f;
    float randomx = (((rand() % 100) - 50) / 50.f) * radius;
    float randomy = (((rand() % 100) - 50) / 50.f) * radius + 7.5;
    float randomz = (((rand() % 100) - 50) / 50.f) * radius;

    particle.Position = glm::vec3(randomx, randomy, randomz);
    particle.Velocity = glm::vec3(0.f, -2.f, 0.f);

    particle.axis = glm::normalize(glm::vec3(rand(), rand(), rand()));
    particle.model = rotationParticle(0, particle.axis);

    particle.m_lifeTime = 0.f;

}

void particlesystem::updateParticles(float dt) {

    // increment rotation
    m_rotationIncrement += 2.5f * dt;

    // reset unused particles
    for (auto& particle : particles) {
        if (particle.m_lifeTime > particle.m_maxLifeTime) // lifetime check
            resetParticle(particle);

        if (particle.Position[1] < 0.f) // height check
            resetParticle(particle);
    }

    // wind
    glm::vec3 windGroup = (1.f / 50.f) * glm::vec3(rand() % 100 + 1 - 50);

    // update all particles
    for (auto& particle : particles) {

        // increment lifetime
        particle.m_lifeTime += dt;

        // rotations
        particle.model = rotationParticle(m_rotationIncrement, particle.axis);

        // propogation
        glm::vec3 windIndvidual = (1.f / 50.f) * glm::vec3(rand() % 100 + 1 - 50);
        particle.Velocity += 0.1f * windGroup + 0.05f * windIndvidual;
        particle.Position += particle.Velocity * dt;

    }

}

// draw all particles
void particlesystem::draw(GLuint m_shader) {

    for (auto& particle : particles) {
        glBindVertexArray(m_particle_vao);

        glUniformMatrix4fv(glGetUniformLocation(m_shader, "m_model"), 1, false, &toMat4(particle.model)[0][0]);
        glUniform3fv(glGetUniformLocation(m_shader, "m_offset"), 1, &particle.Position[0]);

        glDrawArrays(GL_TRIANGLES, 0, 12);
        glBindVertexArray(0);
    }

}


