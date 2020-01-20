#pragma once
#include "objectunit.h"


class AnimationSceneBase {
public:
	int LoadObjects(char* filename);
	void Draw(float deltaTime, unsigned int shaderProgram);
	virtual void Process(float deltaTime) = 0;
	virtual ~AnimationSceneBase() = 0;
protected:
	vector<ObjectBase*> m_objects;
};

class AnimationScene : public AnimationSceneBase {
public:
	//int LoadObjects(char *filename);
	//void Draw(float deltaTime, unsigned int shaderProgram);
	void Process(float deltaTime);
	//virtual void RunScene() = 0;
	void RunScene() {};
	~AnimationScene() {}
};