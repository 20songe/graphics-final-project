#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Particle {
    glm::vec3 Position, Velocity;
    glm::mat3 model;
    glm::vec3 axis;

    float m_lifeTime;
    float m_maxLifeTime = 7.5f; // seconds

    Particle() : Position(0.0f), Velocity(0.0f), model(1.0f) { }
};

class particlesystem
{
public:
    particlesystem();
    particlesystem(unsigned int n); // number of particles

    void init();
    void updateParticles(float dt);
    void draw(GLuint m_shader);

private:
    std::vector<Particle> particles;

    unsigned int n;
    float m_rotationIncrement = 0.f;

    unsigned int m_particle_vbo;
    unsigned int m_particle_vao;

    void resetParticle(Particle &particle);

};

#endif // PARTICLESYSTEM_H
