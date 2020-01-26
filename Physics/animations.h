#pragma once
#include "objectunit.h"
#include "wx/wxprec.h"
#include "GL/glew.h"
#include <GL/glu.h> 

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

class SplashScreenScene : public AnimationSceneBase {
public:
	int LoadObjects(char *filename);
	//void Draw(float deltaTime, unsigned int shaderProgram);
	void Process(float deltaTime);
	void Initialize();
	void Replay();
	//virtual void RunScene() = 0;
	void RunScene() {};
	~SplashScreenScene();
};

class CollisionScene : public AnimationSceneBase {

public:	
	int LoadObjects(char* filename);
	void Process(float deltaTime) {}
	void Initialize() {}
	void Replay() {}
	void RunScene() {}
	~CollisionScene() {}
};