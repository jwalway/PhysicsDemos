#pragma once
#include "objectunit.h"


class AnimationScene{
public:
	int LoadObjects(char *filename);
	void Draw(float deltaTime, unsigned int shaderProgram);
	//virtual void RunScene() = 0;
	void RunScene() {};
	~AnimationScene();
private:
	vector<ObjectUnit *> m_objects;

};