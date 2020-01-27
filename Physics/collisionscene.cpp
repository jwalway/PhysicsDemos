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

#include "collisionscene.h"

int CollisionScene::LoadObjects(char* filename)
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
        if (elm == "<backgroundtexture>")
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
        else if (elm == "</backgroundtexture>")
        {
            objectData.push_back(elm);
            //create object and send data to it
            //ObjectUnit* objUnit = new ObjectUnit();
            m_backgroundObject = new BackgroundObject();
            m_backgroundObject->LoadObject(objectData);
           
            //objUnit->LoadObject(objectData);
            //m_objects.push_back(objUnit);
        }
        else {
            objectData.push_back(elm);
        }
    }
    LoadShaders(vertexShader.c_str(), fragmentShader.c_str());

    return 0;
}

void CollisionScene::Initialize()
{
    m_backgroundObject->InitObject();
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(m_shaderProgram);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "texture2"), 1);
}

void CollisionScene::Draw(float deltaTime)
{
    UseShaderProgram();
    m_backgroundObject->Draw(deltaTime, m_shaderProgram);
}

CollisionScene::~CollisionScene()
{
    if (m_backgroundObject != nullptr)
    {
        delete m_backgroundObject;
    }
}