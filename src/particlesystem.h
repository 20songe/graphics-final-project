#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Particle {
    glm::vec3 Position, Velocity;
    glm::mat3 model;
    glm::vec3 axis;

    bool m_hit = false;

    float m_lifeTime;
    float m_maxLifeTime = 15.f; // seconds

    Particle() : Position(0.0f), Velocity(0.0f), model(1.0f) { }
};

class particlesystem
{
public:
    particlesystem();
    particlesystem(unsigned int n); // number of particles

    void init();
    std::vector<glm::vec3> updateParticles(float dt);
    void draw(GLuint m_shader);

private:
    std::vector<Particle> particles;

    unsigned int n;

    unsigned int m_particle_vbo;
    unsigned int m_particle_vao;

    void resetParticle(Particle &particle);

};

#endif // PARTICLESYSTEM_H
