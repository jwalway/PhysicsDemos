#pragma once

#include "GL/glew.h"
#include <GL/glu.h> 
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"


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
	void Update(float deltaTime);
	GLuint LoadTexture(const char* imagepath);
	void SetShader(unsigned int shader) { m_shaderProgram = shader; }
	void SetTexture(unsigned int texture) {
		m_texture = texture;
	}
	void SetProjection(glm::mat4& proj) { m_projection = proj; }
	void Init();
private:
	vector<Particle> m_particles;
	glm::vec2 m_position;
	unsigned int m_VBO, m_VAO, m_EBO;
	int m_numberOfParticles = 1000;
	unsigned int m_shaderProgram;
	unsigned int m_texture = 0;
	string m_textureFile = "";
	glm::mat4 m_projection;
	void RespawnParticle(Particle& particle);
};

