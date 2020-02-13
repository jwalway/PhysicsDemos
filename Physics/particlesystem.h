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

#pragma once

#include "GL/glew.h"
#include <GL/glu.h> 
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "resources.h"

using namespace std;

struct Particle {
	glm::vec3 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }

};

class ParticleGenerator {
public:

	void Draw(float deltaTime);
	void Update(float deltaTime, glm::vec3 objectPosition = { 0,0,0 });
	GLuint LoadTexture(const char* imagepath)
	{
		m_texture = m_resources->LoadTexture(imagepath);
		return m_texture;
	}
	void LoadShaders(const char* vertexFile, const char* fragmentFile)
	{
		m_shaderProgram = m_resources->LoadShaders(vertexFile, fragmentFile);
	}
	void SetShader(unsigned int shader) { m_shaderProgram = shader; }
	void SetTexture(unsigned int texture) {
		m_texture = texture;
	}
	void SetProjection(glm::mat4& proj) { m_projection = proj; }
	void SetProjection(int width, int height)
	{

	}
	void SetCanvasSize(int w, int h)
	{
		m_width = w;
		m_height = h;
		m_projection = glm::ortho(0.0f, static_cast<GLfloat>(w), static_cast<GLfloat>(h), 0.0f, -1.0f, 1.0f);
	}
	void Init();
	void AddResourceManager(shared_ptr<ResourceManager> rm) {
		m_resources = rm;
	}

	~ParticleGenerator();
private:
	vector<Particle> m_particles;
	glm::vec2 m_position;
	unsigned int m_VBO, m_VAO, m_EBO;
	int m_numberOfParticles = 1000;
	unsigned int m_shaderProgram;
	unsigned int m_texture = 0;
	string m_textureFile = "";
	glm::mat4 m_projection;
	glm::vec3  m_objectPosition { 0 };
	void RespawnParticle(Particle& particle);
	//ResourceManager *m_resources=nullptr;
	shared_ptr<ResourceManager> m_resources;
	int m_width, m_height;
};

