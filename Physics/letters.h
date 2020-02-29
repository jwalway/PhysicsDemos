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

#include "GL/glew.h"
#include <GL/glu.h> 
#include <string>
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "resources.h"


class Particle2 {
public:
    Particle2();
    ~Particle2();

public:
    glm::vec2 position;     // 8 bytes
    glm::vec2 forces;       // 8 bytes  - offset 8
    glm::vec2 velocity;     // 8 bytes  - offset 16
    float mass;        // 4 bytes  - offset 24
    float inv_mass;    // 4 bytes  - offset 28 
    float size;        // 4 bytes  - offset 32
    glm::vec4 color;   // 12 bytes - offset 36
    glm::vec2 offset;  // for particle dynamics
    float life;
    float resetLife;
};

class PiecewiseCurve {
public:
    void SetPeriod(float period)
    {
        m_period = period;
        m_currentTime = 0.0f;       
    }

    void AddPoints(initializer_list<glm::vec2> pts);
    void RemovePoints() { m_points.clear(); }
    glm::vec2 PlayCurve(float deltaTime);
private:
    float m_period;   
    float m_currentTime;
    std::vector<glm::vec2> m_points;
};

class LetterParticles {
    glm::vec2 m_position;     // 8 bytes    
    glm::vec2 m_velocity;     // 8 bytes  - offset 16
public:
    LetterParticles();
    void SetPath(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float period)
    {
        m_pts[0] = p0;
        m_pts[1] = p1;
        m_pts[2] = p2;
        m_pts[3] = p3;
        m_period = period; 
        m_timeRemaining = m_period;
    }
    void CreatePath();
    void CreatePathShape(glm::vec2 endPt, int shape,int invert,float period);
    void Add(Particle2 wd);
    void DuplicateLetter()
    {
        m_base.clear();
        for (auto v : m_letter)
        {
            m_base.push_back(v);
        }
    }
    void Clear();
    void GetLetter(std::vector<Particle2>& ltr)
    {
        for (auto v : m_letter)
        {
            ltr.push_back(v);
        }

        for (auto p : m_path)
        {
            ltr.push_back(p);
        }
    }

    void SetScreen(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    void ScaleToScreen(glm::vec2& pt);
  
    void Update(float deltaTime);
    void SetVelocity(float dx, float dy)
    {
        m_velocity.x = dx;
        m_velocity.y = dy;
    }
    void Reset() { m_timeRemaining = m_period; }
    void SetPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
    }
private:
    std::vector<Particle2> m_letter;  //world position of the letter
    std::vector<Particle2> m_base; //Sits in relative position for tanslation into world position
    std::vector<Particle2> m_path; //Bezier path
    glm::mat4 m_transform;
    int m_width, m_height;
    float m_angle = 0.0f;
    float m_alpha = 0.5f;
    float m_period; //total time over which the letter moves from starting point to ending point.
    float m_timeRemaining; //time remaining in seconds to complete path.
    glm::vec2 m_pts[4];
    PiecewiseCurve m_dynamicsCurve;
    glm::vec2 BezierPath(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t);
    //float m_p0, m_p1, m_p2, m_p3; //Control points for the Bezier path
};

//Spells out "PHYSICS SIMULATIONS"
class WordsParticles {

public:
    WordsParticles();
    void SetResourceManager(shared_ptr<ResourceManager> resources)
    {
        m_resources = resources;
    }
    ~WordsParticles();
    bool Setup(int w, int h);
    void Update(float dt = 0.016f);
    void Replay() 
    {
        for (auto& v : m_letters)
        {
            v.Reset();
        }
    };
    void Draw();
    void AddDrop(glm::vec2 position, float mass);
    void SetShader(unsigned int shader) { m_shaderProgram = shader; }
    void SetCanvasSize(int w, int h)
    {
        win_w = w;
        win_h = h;
        for (auto& v : m_letters)
        {
            v.SetScreen(w, h);
        }
    }
    GLuint LoadTexture2(const char* imagepath);
    GLuint LoadTexture(const char* imagepath)
    {
        m_texture = m_resources->LoadTexture(imagepath);
        return m_texture;
    }
    void LoadShaders(const char* vertexFile, const char* fragmentFile)
    {
        m_shaderProgram = m_resources->LoadShaders(vertexFile, fragmentFile);
    }

    void SetTexture(unsigned int texture) { m_texture = texture; }
    void ScaleToScreen(glm::vec2& pt);
private:
    int win_w;
    int win_h;
    float m_size = 4.0f;
    unsigned int prog;                               /* shaders / prog */
    unsigned int m_shaderProgram;
    unsigned int m_texture = 0;
    std::string m_textureFile = "";
    GLuint vbo;                                 /* the vbo that holds the water drop data */
    GLuint vao;                                 /* vertex array object */
    size_t bytes_allocated;                     /* number of bytes we allocted on gpu */
    std::vector<Particle2> drops;               /* the water drop particles */
    glm::mat4 pm;                                    /* projection matrix; ortho */
    std::vector<LetterParticles> m_letters;// , m_P2, m_P3, m_P4;
    void Make_P(glm::vec2 pos,float scale, float dInc);
    void Make_H(glm::vec2 pos, float scale, float dInc);
    void Make_Y(glm::vec2 pos, float scale, float dInc);
    void Make_I(glm::vec2 pos, float scale, float dInc);
    void Make_M(glm::vec2 pos, float scale, float dInc);
    void Make_L(glm::vec2 pos, float scale, float dInc);
    void Make_A(glm::vec2 pos, float scale, float dInc);
    void Make_T(glm::vec2 pos, float scale, float dInc);
    void Make_N(glm::vec2 pos, float scale, float dInc);
    void Make_O(glm::vec2 pos, float scale, float dInc);
    void Make_C(glm::vec2 pos, float scale, float dInc);
    void Make_U(glm::vec2 pos, float scale, float dInc);
    void Make_S(glm::vec2 pos, float scale, float dInc);
    //float m_range1 = 100.0, m_range2 = 100.0f;//2.0 and 10.0f;
    float m_range1 = 1.0, m_range2 = 3.0f; // 0.05, 0.1
    float m_rangeA = 0.01f, m_rangeB = 0.03f; // 0.05, 0.1
    void CreateLetters();
    void HorizontalLine(LetterParticles& p, glm::vec2 ptStart, float xEnd,float scale=50.0f, float dInc=0.01);
    void VerticalLine(LetterParticles& p, glm::vec2 ptStart, float yEnd, float scale=50.0f, float dInc=0.01f);

    void HorizontalLine2(LetterParticles& p, glm::vec2 ptStart, float xEnd, float scale = 50.0f, float dInc = 0.01);
    void VerticalLine2(LetterParticles& p, glm::vec2 ptStart, float yEnd, float scale = 50.0f, float dInc = 0.01f);

    void DiagonalLine(LetterParticles& p, glm::vec2 ptStart, glm::vec2 ptEnd, float scale = 50.0f, float dInc = 0.01f);
    void EllipseLine(LetterParticles& p, glm::vec2 ptStart, float major, float minor, float angStart, float angEnd, float scale = 50.0f, float dInc = 0.01f);
    shared_ptr<ResourceManager> m_resources;
};