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

#include "splashscene.h"
#include "zebra.xpm"

float positiveDelta(float x1, float x2)
{
    if (x1 < x2)
        return x2 - x1;
    return x1 - x2;
}

void SplashScene::Initialize()
{
    for (auto& x : m_objects)
    {
        x->InitObject();
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(m_shaderProgram);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "texture2"), 1);
}

void SplashScene::Replay()
{
    for (auto& x : m_objects)
        x->Replay();
}


int SplashScene::LoadObjects(char* filename)
{
    //Parsing xml *.data file for the animation scene
    string dataElement;
    string vertexShader = "", fragmentShader = "";
    ifstream animationFile;
    //std::stringstream shaderStream;
    vector<string> dataElements;

    animationFile.open(filename);
    if (!animationFile) {
        // wxMessageBox(wxString::Format("Was unable to open %s!", filename), wxT("Error"));
        return -1;
    }

    while (std::getline(animationFile, dataElement))
    {
        trim(dataElement);
        dataElements.push_back(dataElement);
    }
    animationFile.close();

    //Sanity check of dataElements should be done  (no spaces between lines and matching tags should exist) 1/17/20
    //I could also determine Object Type here from the first tag in the animation.data xml file 1/19/20, 10:15 p.m.

    deque<string> objectData;

    for (string& elm : dataElements)
    {
        if (elm == "<object>")
        {
            //Start of an object
            objectData.clear();
            objectData.push_back(elm);
        }
        else if (elm == "<vertexshader>")
        {
            objectData.clear();
        }
        else if (elm == "</vertexshader>") {
            vertexShader = objectData[0];
        }
        else if (elm == "<fragmentshader>")
        {
            objectData.clear();
        }
        else if (elm == "</fragmentshader>")
        {
            fragmentShader = objectData[0];
        }
        else if (elm.substr(0, 4) == "<!--")
        { // skip comments
            continue;
        }
        else if (elm == "</object>")
        {
            objectData.push_back(elm);
            //create object and send data to it
            ObjectUnit* objUnit = new ObjectUnit();
            //ObjectBase* objBase = objUnit;
            objUnit->LoadObject(objectData);
            m_objects.push_back(objUnit);
        }
        else {
            objectData.push_back(elm);
        }
    }
    LoadShaders(vertexShader.c_str(), fragmentShader.c_str());

    return 0;
}

void SplashScene::Draw(float deltaTime) 
{
    UseShaderProgram();   
    for (auto& x : m_objects)
    {
        x->Draw(deltaTime, m_shaderProgram);
    }
}

void SplashScene::Process(float deltaTime)
{

}

void SplashScene::Description(wxRichTextCtrl& r)
{
    r.Clear();
    r.Freeze();
    r.BeginSuppressUndo();
    r.BeginParagraphSpacing(0, 20);
    r.BeginAlignment(wxTEXT_ALIGNMENT_CENTER);
    r.BeginBold();
    r.BeginFontSize(14);

    wxString lineBreak = (wxChar)29;

    r.WriteText(wxString(wxT("Physics Simulations")));
    r.EndFontSize();

    r.EndBold();

    r.Newline();

    r.BeginFontSize(12);
    r.WriteText(wxT("Learn Physics Concepts and See Them Simulated.\n"));

    r.BeginItalic();
    r.WriteText(wxT("by John Alway"));
    r.EndItalic();
    r.EndFontSize();
    r.Newline();
    r.WriteImage(wxBitmap(zebra_xpm));

    r.Newline();
    r.Newline();

    r.EndAlignment();
    r.EndParagraphSpacing();
    r.EndSuppressUndo();
    r.Thaw();
}

SplashScene::~SplashScene()
{
    int val = (int)glGetError();
    for (auto& x : m_objects)
    {
        delete x;
    }
}
