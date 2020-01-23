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

using namespace std;


class ObjectBase {
public:
	virtual void Calculate(float deltaTime) = 0;
	virtual void Draw(float deltaTime, unsigned int shaderProgram) = 0;
	virtual int LoadObject(deque<string>& objectData) = 0;
	virtual void Replay() = 0;
	virtual void InitObject() = 0;
	virtual ~ObjectBase() = 0 {};
};

class ObjectUnit: public ObjectBase {
public:

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

	glm::vec3 getVelocity() { return m_velocity; }
	glm::vec3 getPosition() { return m_position; }
	glm::vec3 getGravityWell() { return m_gravityWell; }
	~ObjectUnit();
protected:
	GLuint LoadTexture(const char* imagepath);
private:
	glm::mat4 m_trans;
	unsigned int m_VAO;
	unsigned int m_shaderProgram;
	float *m_vertices;
	int m_verticesSize;
	int  *m_indices;
	int m_indicesSize;
	unsigned int m_texture1=0, m_texture2=0;
	string m_textureFile1="", m_textureFile2="";
	glm::vec3 m_initPosition;
	glm::vec3 m_initVelocity;
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_gravityWell;
	float m_inflateValue=0.5f;

};
