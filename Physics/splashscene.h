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

#include "animations.h"
#include "letters.h"

class SplashScene : public AnimationSceneBase {
public:
	int LoadObjects(char* filename);
	void Draw(float deltaTime);
	void Process(float deltaTime);
	void Initialize(int state);
	void Replay();
	void Description(wxRichTextCtrl& r);
	void SetCanvasSize(int w, int h)
	{
		if (m_wordsParticles != nullptr) {
			m_wordsParticles->SetCanvasSize(w, h);
		}
		AnimationSceneBase::SetCanvasSize(w, h);
	}
	//virtual void RunScene() = 0;
	void RunScene() {};
	~SplashScene();
private:
	shared_ptr<WordsParticles> m_wordsParticles;
};

enum class Splash
{
	List = 10000
};
