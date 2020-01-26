#pragma once
#include "objectunit.h"
#include "wx/wxprec.h"
#include "GL/glew.h"
#include <GL/glu.h> 
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include "wx/defs.h"
#include "wx/app.h"

void trim(string& str, string trimchars = " \t\f\v\n\r");

class AnimationSceneBase {
public:
	virtual int LoadObjects(char* filename) = 0;
	void Draw(float deltaTime);
	virtual void Process(float deltaTime) = 0;
	void LoadShaders(const char* vertexFile, const char* fragmentFile);
	virtual void Initialize()=0;
	virtual void Replay() = 0;
	virtual ~AnimationSceneBase() = 0;	
	void UseShaderProgram() { glUseProgram(m_shaderProgram); }	
protected:
	vector<ObjectBase*> m_objects;
	unsigned int m_shaderProgram;
	unsigned int CompileShader(const char* filename, unsigned int shaderType);
	unsigned int LinkShaders(unsigned int vertex, unsigned int fragment);
};


