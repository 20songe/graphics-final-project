#include "particleemitter.h"
#include "src/shaderloader.h"

ParticleEmitter::ParticleEmitter()
{
    this->init();
}

void ParticleEmitter::update(float dt)
{
    // update all particles
    //FIXME for loop is wrong
    for (unsigned int i = 0; i < this->maxParticles; ++i)
    {
        Particle &p = this->particles[i];
        p.timeLeft -= dt; // reduce life
        if (p.timeLeft > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
        }
    }
}

// render all particles
void ParticleEmitter::draw()
{
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glUseProgram(m_shader);


    glm::mat4 identity(1);
    GLint uniModel = glGetUniformLocation(m_shader, "model");
    //fixme what needs to be passed in for the positions?
    for (Particle particle : this->particles)
    {
        if (particle.timeLeft > 0.0f)
        {
            //this->texture.Bind();
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleEmitter::init()
{
    // The VBO containing the 4 vertices of the particles.
    // Thanks to instancing, they will be shared by all particles.
    float g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
    };

    int elems[] = {
        0, 1, 2,
        2, 3, 0
    };

    m_shader = ShaderLoader::createShaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    glGenBuffers(1, &particle_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, particle_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    //glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

    //TODO add uv attribute
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void*)(3 * sizeof(float)));

}

//// stores the index of the last particle used (for quick access to next dead particle)
//unsigned int lastUsedParticle = 0;
//unsigned int ParticleEmitter::firstUnusedParticle()
//{
//    // first search from last used particle, this will usually return almost instantly
//    for (unsigned int i = lastUsedParticle; i < this->amount; ++i){
//        if (this->particles[i].Life <= 0.0f){
//            lastUsedParticle = i;
//            return i;
//        }
//    }
//    // otherwise, do a linear search
//    for (unsigned int i = 0; i < lastUsedParticle; ++i){
//        if (this->particles[i].Life <= 0.0f){
//            lastUsedParticle = i;
//            return i;
//        }
//    }
//    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
//    lastUsedParticle = 0;
//    return 0;
//}

//void ParticleEmitter::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset)
//{
//    float random = ((rand() % 100) - 50) / 10.0f;
//    float rColor = 0.5f + ((rand() % 100) / 100.0f);
//    particle.Position = object.Position + random + offset;
//    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
//    particle.Life = 1.0f;
//    particle.Velocity = object.Velocity * 0.1f;
//}

//ParticleEmitter::ParticleEmitter()
//{

//}
