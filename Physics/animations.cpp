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

#include "animations.h"

void trim(string &str, string trimchars)
{
    //const string whitespace = " \t\f\v\n\r";
    //string test = "    test1 test2 test3     \n\n";
    int start = (int)str.find_first_not_of(trimchars);
    int end = (int)str.find_last_not_of(trimchars);
    str.erase(0, start);
    str.erase((end - start) + 1);    
}


void AnimationSceneBase::LoadShaders(const char* vertexFile, const char* fragmentFile)
{
    if (m_resources.DoesResourceExist(vertexFile, fragmentFile)) {
        m_shaderProgram = m_resources.GetResourceID(vertexFile, fragmentFile);
        return;
    }
    unsigned int vshader = CompileShader(vertexFile, GL_VERTEX_SHADER);
    if (!vshader) return;
    unsigned int fshader = CompileShader(fragmentFile, GL_FRAGMENT_SHADER);
    if (!fshader) return;
    unsigned int shaderProgram = LinkShaders(vshader, fshader);
    if (!shaderProgram) return;
    m_shaderProgram = shaderProgram;
    m_resources.AddResource(vertexFile, fragmentFile, DataType::shader, m_shaderProgram);
}

unsigned int AnimationSceneBase::LinkShaders(unsigned int vertex, unsigned int fragment)
{
    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[1024];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) { //Error
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        wxMessageBox(wxString::Format("%s!", infoLog), wxString::Format("Shader Link Error"));
        return 0;
    }
    m_shaderProgram = shaderProgram;

    //After creating program, the shader objects are no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return m_shaderProgram;
}

// Pass the filename of the shader to be compiled, and the shader type, which could be
// either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
unsigned int AnimationSceneBase::CompileShader(const char* filename, unsigned int shaderType)
{
    // 1. retrieve the vertex/fragment source code from filePath
    string shaderCode;
    ifstream shaderFile;
    std::stringstream shaderStream;

    shaderFile.open(filename);

    if (!shaderFile) {
        wxMessageBox(wxString::Format("Was unable to open %s!", filename), wxT("Error"));
        return 0;
    }

    // read file's buffer contents into streams
    shaderStream << shaderFile.rdbuf();
    // close file handlers
    shaderFile.close();

    // convert stream into string
    shaderCode = shaderStream.str();
    const char* shaderCodeStr = shaderCode.c_str();
    unsigned int shaderObject;

    shaderObject = glCreateShader(shaderType); // GL_VERTEX_SHADER);
    glShaderSource(shaderObject, 1, &shaderCodeStr, NULL);
    glCompileShader(shaderObject);

    int success;
    char infoLog[1024];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if (!success)
    { //Error
        string str;
        if (shaderType == GL_VERTEX_SHADER)
            str = "Vertex";
        else
            str = "Fragment";
        glGetShaderInfoLog(shaderObject, 1024, NULL, infoLog);
        //std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        wxMessageBox(wxString::Format("%s!", infoLog), wxString::Format("%s Shader Compile Error", str.c_str()));
        return 0;
    }
    return shaderObject;
}

/*
void AnimationSceneBase::Draw(float deltaTime)
{  
    UseShaderProgram(); 
    for (auto& x : m_objects)
    {
        x->Draw(deltaTime, m_shaderProgram);               
    }
}
*/

AnimationSceneBase::~AnimationSceneBase()
{    
    glDeleteProgram(m_shaderProgram);
}