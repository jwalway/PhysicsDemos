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

class ObjectUnit {
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
	~ObjectUnit();
protected:
	GLuint LoadTexture(const char* imagepath);
private:
	unsigned int m_VAO;
	unsigned int m_shaderProgram;
	float *m_vertices;
	int m_verticesSize;
	int  *m_indices;
	int m_indicesSize;
	unsigned int m_texture1, m_texture2;
	string m_textureFile1="", m_textureFile2="";
	glm::vec3 m_position;
	glm::vec3 m_velocity;

};
