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
#include "objectunit.h"
#include "wx/wxprec.h"
#include "GL/glew.h"
#include <GL/glu.h> 
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include "wx/defs.h"
#include "wx/app.h"

void trim(string& str, string trimchars = " \t\f\v\n\r");

class AnimationSceneBase {
public:
	map<string, wxControl*> *m_controls;
	virtual int LoadObjects(char* filename) = 0;
	virtual void Draw(float deltaTime) = 0;
	virtual void Process(float deltaTime) = 0;
	void LoadShaders(const char* vertexFile, const char* fragmentFile);
	virtual void Initialize()=0;
	virtual void Replay() = 0;
	virtual ~AnimationSceneBase() = 0;	
	void UseShaderProgram() { glUseProgram(m_shaderProgram); }
	void SetPanelControls(map<string, wxControl*>& controls) {
		m_controls = &controls;
	}
protected:
	vector<ObjectBase*> m_objects;
	unsigned int m_shaderProgram;
	unsigned int CompileShader(const char* filename, unsigned int shaderType);
	unsigned int LinkShaders(unsigned int vertex, unsigned int fragment);
};


