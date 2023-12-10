/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "particlegenerator.h"
#include "src/shaderloader.h"
#include <iostream>


ParticleGenerator::ParticleGenerator()
{
}

ParticleGenerator::ParticleGenerator(unsigned int amount):
    amount(amount)
{
    this->init();
}

void ParticleGenerator::Update(float dt, unsigned int newParticles, glm::vec3 offset)
{
    // add new particles
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
        }
    }
}

// render all particles
void ParticleGenerator::Draw()
{
    // use additive blending to give it a 'glow' effect
    glUseProgram(shader);
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
//            this->shader.SetVector2f("offset", particle.Position);
//            this->shader.SetVector4f("color", particle.Color);
            //this->texture.Bind();
            //TODO add textures
            glUniform3fv(glGetUniformLocation(shader, "offset"), 1, &particle.Position[0]);
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // don't forget to reset to default blending mode
    glUseProgram(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void ParticleGenerator::init()
{
    // set up mesh and attribute properties
    unsigned int VBO;
//    float particle_quad[] = {
//        0.0f, 1.0f, 0.0f, 1.0f,
//        1.0f, 0.0f, 1.0f, 0.0f,
//        0.0f, 0.0f, 0.0f, 0.0f,

//        0.0f, 1.0f, 0.0f, 1.0f,
//        1.0f, 1.0f, 1.0f, 1.0f,
//        1.0f, 0.0f, 1.0f, 0.0f
//    };
    float particle_quad[] = {
        0.0f, 1.0f, 0.f,
        1.0f, 0.0f, 0.f,
        0.0f, 0.0f, 0.f,

        0.0f, 1.0f, 0.f,
        1.0f, 1.0f, 0.f,
        1.0f, 0.0f, 0.f
    };
    shader = ShaderLoader::createShaderProgram("resources/shaders/particles.vert", "resources/shaders/particles.frag");
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, glm::vec3 offset)
{
    //float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    // game object was used for position and velocity here?
    particle.Position = glm::vec3(0.f) + offset;

    particle.Life = 1.0f;
    particle.Velocity = glm::vec3(0.f) * 0.1f;
}
