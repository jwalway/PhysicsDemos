/////////////////////////////////////////////////////////////////////////////
// Name:        Physics Demos Program
// Purpose:     To Simulate Physics Principles
// Author:      John Alway
//
// Created: 1/13/2020
// Copyright 2020 John Alway. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE.TXT file.
/////////////////////////////////////////////////////////////////////////////

#include "particlesystem.h"

#include <time.h> 


void ParticleGenerator::Init()
{
    m_numberOfParticles = 250;
    srand(time(NULL));
    unsigned int VBO, VAO;
    // Set up mesh and attribute properties
    //GLuint VBO;
    GLfloat particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    m_VAO = VAO;
    m_VBO = VBO;
    //m_EBO = EBO;
    //val = (int)glGetError();
    for (int i = 0; i < m_numberOfParticles; i++)
    {
        m_particles.push_back(Particle());
    }
}

void ParticleGenerator::Draw(float deltaTime)
{
    int val = (int)glGetError();
    static float delta = 0.0f;
    return;
    delta += 0.1f;
    if (delta > 25.0f)
        delta = 0.0f;
    static float dx = 470.0 / 2, dy = 427 / 2.0;



    // Use additive blending to give it a 'glow' effect
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //this->shader.Use();
    glUseProgram(m_shaderProgram);
    for (Particle particle : m_particles)
    {
        if (particle.Life >= 0.0f)
        {
            glm::vec4 clr = particle.Color;
            glm::vec2 offset = glm::vec2(particle.Position);

            glm::mat4 trans = glm::mat4(1.0f);
            static float deg = 0.0f;

            deg += 0.3f;
            if (deg >= 360.0f)
                deg = 0.0f;
            //trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
            //trans = glm::translate(m_projection, glm::vec3(1.0, -1.0, 0.0));
            trans = m_projection;
            trans[1].y *= -1.0f;
            trans[3].x = 0.0f;
            trans[3].y = 0.0f;
            //trans = glm::translate(m_projection, glm::vec3(470.0/2, 427/2.0, 0.0));
            //trans = glm::translate(m_projection, glm::vec3(dx, dy, 0.0));
            trans = glm::translate(trans, glm::vec3(m_objectPosition.x * (m_width / 2), m_objectPosition.y * (m_height / 2), m_objectPosition.z));
            dx += deltaTime * 0.1f;
            dy += deltaTime * 0.1f;
            if (dx > 460.0)
                dx = 10.0f;
            if (dy > 417)
                dy = 10.0f;
            //trans = glm::translate(m_projection, glm::vec3(100.0, 0.0, 0.0));
            //trans = glm::scale(trans, glm::vec3(120.0f, 120.0f, 1.0f));
           // trans = glm::rotate(trans, glm::radians(deg), glm::vec3(0.0, 0.0, 1.0));
            //trans = glm::scale(trans, glm::vec3(0.25 , 0.32, 1.0));
           // trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 1.0f));

            unsigned int transformLoc = glGetUniformLocation(m_shaderProgram, "projection");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

            // offset.x = 0.0f; // + delta;
            // offset.y = 0.0f; // + delta;
            int vertexOffsetLocation = glGetUniformLocation(m_shaderProgram, "offset");
            // glUseProgram(m_shaderProgram);
            glUniform2f(vertexOffsetLocation, offset.x, offset.y); // clr.g, clr.b, clr.a);
            val = (int)glGetError();
            int vertexColorLocation = glGetUniformLocation(m_shaderProgram, "color");
            //  glUseProgram(m_shaderProgram);
              //clr.r = 1.0f;
             // clr.g = 1.0f;
             // clr.b = 0.0f;
              //clr.a = 1.0f;
            glUniform4f(vertexColorLocation, clr.r, clr.g, clr.b, clr.a);  // 0.0f, greenValue, 0.0f, 1.0f);

            //this->shader.SetVector2f("offset", particle.Position);
            //this->shader.SetVector4f("color", particle.Color);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture);
            //this->texture.Bind();
            glBindVertexArray(m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            val = (int)glGetError();
        }
        // break;
    }
    // Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Update(float deltaTime, glm::vec3 objectPosition, float radius)
{
    int i = 0;
    float d = 6.28f;
    //static float radius = 10.0f;
    float angle = 0.0f;
    float green = 1.0f;

    m_radius = radius;
    m_objectPosition = objectPosition;
    //Respawn some new particles
    int newParticles = 2;

    for (int i = 0; i < newParticles; i++) {
        //find unused particle
        for (int j = 0; j < m_particles.size(); j++) {
            if (m_particles[j].Life <= 0.0f)
            {
                RespawnParticle(m_particles[j]);
                break;
            }

        }
    }

    //update all particles
    for (int i = 0; i < m_particles.size(); i++) {
        Particle& p = m_particles[i];
        p.Life -= deltaTime; //reduce life
        if (p.Life > 0.0f) {
            p.Position += p.Velocity * deltaTime;
            p.Color.g -= deltaTime * 0.5f;
            p.Color.a -= deltaTime * 0.5f;// *2.5;
            if (p.Color.a < 0.0f) p.Color.a = 0.0f;
            if (p.Color.g < 0.0f) p.Color.g = 0.0f;
        }
    }
    
}

void ParticleGenerator::RespawnParticle(Particle& particle)
{
    float d = 10.0f; // 6.28f;
    float radius = m_radius; // 12.0f;
    float angle = 0.0f;
    float scale = 6.28f / (float)RAND_MAX;
    float dv;
    angle = scale * rand();
    d = rand() * (30.0f / (float)RAND_MAX);
    dv = rand() * (2.0f / (float)RAND_MAX);
    particle.Position.x = radius * glm::cos(angle);
    particle.Position.y = radius * glm::sin(angle);
    particle.Color.r = 1.0f; // scale* rand();
    particle.Color.g = 1.0f; // scale* rand();
    particle.Color.b = 0.0f; // scale* rand();
    particle.Color.a = 1.0f;
    particle.Life = 1.0f * dv;
    particle.Velocity.x = glm::cos(angle) * d;
    particle.Velocity.y = glm::sin(angle) * d;
    particle.Velocity.z = 0.0f;
}
/*
GLuint ParticleGenerator::LoadTexture(const char* imagepath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
    GLuint textureID;

    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // Give the image to OpenGL
    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    }
    else {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    stbi_image_free(data);
    return textureID;
}
*/

ParticleGenerator::~ParticleGenerator()
{
    //The ResourceManager deletes the textures and shader programs
    //unsigned int texs[2] = { m_texture, 0};
    //glDeleteTextures(2, texs);
    glDeleteVertexArrays(1, &m_VAO);  
    glDeleteBuffers(1, &m_VBO);
}