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
#include <vector>
#include <string>
#include <vector>
#include <deque>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particlesystem.h"
#include "resources.h"

using namespace std;


class ObjectBase {
public:
	ObjectBase(shared_ptr<ResourceManager> rm)
	{
		m_resources = rm;
	}
	virtual void Calculate(float deltaTime) = 0;
	virtual void Draw(float deltaTime, unsigned int shaderProgram) = 0;
	virtual int LoadObject(deque<string>& objectData) = 0;
	GLuint LoadTexture(const char* imagepath)
	{
		return m_resources->LoadTexture(imagepath);
	}
	virtual void SetState(int state) = 0;
	virtual void Replay() = 0;
	virtual void InitObject() = 0;
	virtual ~ObjectBase() = 0 {};	
	virtual void SetCanvasSize(int w, int h)
	{
		m_width = w;
		m_height = h;
	}
	/*
	void SetResourceManager(shared_ptr<ResourceManager> rm)
	{
		m_resources = rm;
	}*/

protected:
	int m_width, m_height;
	//ResourceManager* m_resources;
	shared_ptr<ResourceManager> m_resources;
};

class BackgroundObject : public ObjectBase
{
public:
	BackgroundObject(shared_ptr<ResourceManager> rm) : ObjectBase(rm) 
	{
		//m_resources = rm;
	}
	void Calculate(float deltaTime);
	void Draw(float deltaTime, unsigned int shaderProgram);
	int LoadObject(deque<string>& objectData);
	//GLuint LoadTexture(const char* imagepath);
	void SetState(int state) {};
	void Replay() {};
	void InitObject();
	~BackgroundObject();
private:
	glm::mat4 m_trans;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;
	unsigned int m_shaderProgram;
	float* m_vertices;
	int m_verticesSize;
	int* m_indices;
	int m_indicesSize;
	unsigned int m_texture1 = 0, m_texture2 = 0;
	string m_textureFile1 = "", m_textureFile2 = "";
	glm::vec3 m_position;
};

class ObjectUnit: public ObjectBase {
public:
	ObjectUnit(shared_ptr<ResourceManager> rm) : ObjectBase(rm)
	{
		//m_resources = rm;
		m_particleGen = make_shared<ParticleGenerator>();
		m_particleGen->AddResourceManager(rm);
		m_particleGen->Init();
		m_particleGen->LoadTexture("..\\resources\\SplashScene\\particle.png");
		m_particleGen->LoadShaders("..\\resources\\SplashScene\\particle.vert", "..\\resources\\SplashScene\\particle.frag");
	}
	void InitObject();
	unsigned int GetShaderProgram() { return m_shaderProgram; }
	void SetShaderProgram(unsigned int program) { m_shaderProgram = program; }
	unsigned int GetVAO() { return m_VAO; }
	void SetVAO(unsigned int vao) { m_VAO = vao; }
	//unsigned int GetTexture() { return m_texture; }
	//void SetTexture(unsigned int texture) { m_texture = texture; }
	int LoadObject(deque<string> &objectData);
	void Draw(float deltaTime, unsigned int shaderProgram);
	void Calculate(float deltaTime);
	void Replay();
	void SetState(int state);
	void SetCanvasSize(int w, int h)
	{
		ObjectBase::SetCanvasSize(w, h);
		m_particleGen->SetCanvasSize(w, h);
		m_projection = glm::ortho(0.0f, static_cast<GLfloat>(w), static_cast<GLfloat>(h), 0.0f, -1.0f, 1.0f);
	}

	glm::vec3 getVelocity() { return m_velocity; }
	glm::vec3 getPosition() { return m_position; }
	glm::vec3 getGravityWell() { return m_gravityWell; }
	~ObjectUnit();
protected:
	//GLuint LoadTexture(const char* imagepath);
private:
	glm::mat4 m_trans;
	unsigned int m_VAO=0;
	unsigned int m_VBO=0;
	unsigned int m_EBO=0;
	unsigned int m_shaderProgram=0;
	float *m_vertices=nullptr;
	int m_verticesSize=0;
	int  *m_indices=nullptr;
	int m_indicesSize=0;
	unsigned int m_texture1=0, m_texture2=0;
	string m_textureFile1="", m_textureFile2="";
	glm::vec3 m_initPosition {0};
	glm::vec3 m_initVelocity{ 0 };
	glm::vec3 m_position{ 0 };
	glm::vec3 m_velocity{ 0 };
	glm::vec3 m_gravityWell{ 0 };
	float m_inflateValue=0.5f;
	//shared_ptr<ResourceManager> m_resources;
	shared_ptr<ParticleGenerator> m_particleGen;
	glm::mat4 m_projection;
};

