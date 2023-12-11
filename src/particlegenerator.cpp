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

glm::mat3 rotation(float angle, glm::vec3 axis) {

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

std::map<int, glm::vec3> color_map{{1, glm::vec3(0.7, -0.5, -0.7)}, {2, glm::vec3(0.5, 0.f, 0.f)}, {3, glm::vec3(0.2, 0.f, -0.3)}};


void ParticleGenerator::Update(float dt, unsigned int newParticles, glm::vec3 offset)
{
    count += 0.1f;
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
        float height = p.Position.y;

        if (p.Position.y > 0.0f)
        {	// particle is alive, thus update
                glm::mat3 newModel = rotation(count, p.axis);
                p.model = newModel;

                glm::vec3 wind = glm::normalize(glm::vec3(rand() % 100 + 1 - 50, rand() % 100 + 1 - 50, rand() % 100 + 1 - 50))*0.1f;
                p.Position -= (p.Velocity + wind) * dt;

        }
    }
}

// render all particles
void ParticleGenerator::Draw(glm::mat4 view, glm::mat4 proj)
{
    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_color);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_opacity);
    for (Particle particle : this->particles)
    {
        if (particle.Position.y > 0.0f)
        {
            glUniform3fv(glGetUniformLocation(shader, "offset"), 1, &particle.Position[0]);
            glUniform3fv(glGetUniformLocation(shader, "leaf_shade"), 1, &particle.leaf_shade[0]);

            glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix3fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &particle.model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shader, "proj"), 1, GL_FALSE, &proj[0][0]);
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

}

void ParticleGenerator::init()
{

    QImage m_color_image = QImage(QString(":/resources/images/cs1230leaftextures/leaf_color.png"));
    m_color_image = m_color_image.convertToFormat(QImage::Format_RGBA8888).mirrored();

    glGenTextures(1, &texture_color);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_color_image.width(), m_color_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_color_image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    QImage m_normal_image = QImage(QString(":/resources/images/cs1230leaftextures/leaf_normal.png"));
    m_normal_image = m_normal_image.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glGenTextures(1, &texture_normal);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_normal_image.width(), m_normal_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_normal_image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    QImage m_opacity_image = QImage(QString(":/resources/images/cs1230leaftextures/leaf_opacity.png"));
    m_opacity_image = m_opacity_image.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glGenTextures(1, &texture_opacity);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_opacity);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_opacity_image.width(), m_opacity_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_opacity_image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    shader = ShaderLoader::createShaderProgram("resources/shaders/particles.vert", "resources/shaders/particles.frag");

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "leaf_color"), 0);
    glUniform1i(glGetUniformLocation(shader, "normal"), 1);
    glUniform1i(glGetUniformLocation(shader, "opacity"), 2);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);




    GLuint VBO;
    std::vector<GLfloat> particle_quad = {
        -0.1f, 0.0f, 0.1f, 0.f, 1.f,//top left
        -0.1f, 0.0f, -0.1f, 0.f, 0.f, //bottom left
        0.1f, 0.0f, -0.1f, 1.f, 0.f,//bottom right

        0.1f, 0.0f, 0.1f, 1.f, 1.f, //top right
        -0.1f, 0.0f, 0.1f, 0.f, 1.f, //top left
        0.1f, 0.0f, -0.1f, 1.f, 0.f //bottom right
    };



    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, particle_quad.size()*sizeof(GLfloat), particle_quad.data(), GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
        if (this->particles[i].Position.y <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Position.y <= 0.0f){
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
    float randomx = (((rand() % 100) - 50) / 10.f)*0.5;
    float randomz = (((rand() % 100) - 50) / 10.f)*0.5;
    particle.Position = glm::vec3(0.f, 5.f, 0.f) + glm::vec3(randomx, 0.f, randomz);
    particle.axis = glm::normalize(glm::vec3(rand(), rand(), rand()));
    particle.model = rotation(0, particle.axis);
    int randNum = (rand() % 3) + 1;
    particle.leaf_shade = color_map[randNum];
    particle.Velocity = glm::vec3(0.f, 1.f, 0.f) * 0.1f;
}
