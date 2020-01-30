// Copyright 2020 John Alway. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE.TXT file.

#pragma once
#include "animations.h"

class CollisionScene : public AnimationSceneBase {

public:
	void Draw(float deltaTime);
	int LoadObjects(char* filename);
	void Process(float deltaTime) {}
	void Initialize();
	void Replay();
	void Description(wxRichTextCtrl& r);
	void RunScene() {}
	~CollisionScene();
private:
	BackgroundObject* m_backgroundObject;
};

