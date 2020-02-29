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

#include <assert.h>
#include "letters.h"
#include "time.h"
#include "stb_image.h"

float Randv(float startRange = -1.0f, float endRange = 1.0f)
{
    float rslt = 0.0;
    float rnd = (float)rand();
    if (startRange > endRange)
    {
        float temp;
        temp = startRange;
        startRange = endRange;
        endRange = temp;
    }

    rslt = rnd*(endRange - startRange) / (float)RAND_MAX + startRange;
    
    return rslt;
}

Particle2::Particle2() : position(0.0, 0.0), forces(0.0, 0.0), velocity(0.0, 0.0), 
mass(0.0), inv_mass(0.0), size(0.0), resetLife(1.0), life(1.0), color(1.0,0.0,0.0,1.0)

{

}

Particle2::~Particle2() {
}

// ------------------------------------

Words::Words()
    :win_w(0)
    , win_h(0)
    , vbo(0)
    , vao(0)
    , bytes_allocated(0)
{    
}

Words::~Words() {
}

bool Words::Setup(int w, int h) {
    //assert(w && h);
    win_w = w;
    win_h = h;
    CreateLetters();
    srand((unsigned int)time(NULL));
    //pm.ortho(0, w, h, 0, 0.0f, 100.0f);
    pm = glm::ortho(0.0f, static_cast<GLfloat>(w), static_cast<GLfloat>(h), 0.0f, 0.0f, 100.0f); // -1.0f, 1.0f);
    pm[1].y *= -1.0f;
    pm[3].x = 0.0f;
    pm[3].y = 0.0f;
    // create shader 
    const char* atts[] = { "a_pos", "a_size" };
    // prog.create(GL_VERTEX_SHADER, rx_to_data_path("waterdrop.vert"));
    // prog.create(GL_FRAGMENT_SHADER, rx_to_data_path("waterdrop.frag"));
   //  prog.link(2, atts);
    glUseProgram(m_shaderProgram); //glUseProgram(prog.id);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_pm"), 1, GL_FALSE, (const float*)glm::value_ptr(pm));
    float cx = w * 0.5f;
    float cy = h * 0.5f;
    /*
    int num = 10;
    for (int i = 0; i < num; ++i) {
        addDrop(glm::vec2(Random(0, w), Random(0, h)), 1.0f);
    }
    */
    //int err = glGetError();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0); // pos
    glEnableVertexAttribArray(1); // size
    glEnableVertexAttribArray(2); // color   (was lifeOn)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle2), (GLvoid*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle2), (GLvoid*)32);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle2), (GLvoid*)36);

 
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
 
    return true;
}

void Words::CreateLetters()
{
    float scale = 50.0f, dInc = 0.005f;
    float period = 5;
    float period1 = 4, period2 = 8;
    glm::vec2 pos;
    glm::vec2 p0, p1, p2;
    float dx = 30.0f;
    float dy = 10.0f;
    pos.x = -150.0f;
    pos.y = 100.0f;
    p0 = glm::vec2(-160.0f, -300.0f);
    p1 = glm::vec2(-190.0f, -100.0f);
    p2 = glm::vec2(0.0f, 40.0f);

    ////////// 2/27/20, 5:22 p.m.
    pos.x = -0.7f; // -0.8;
    pos.y = 0.2f; // 0.458; // 0.5;
    
    pos.x = -0.55f;
    scale = 0.25f;
    Make_P(pos, 0.25f, dInc);
    
    //m_letters.back().SetPath(p0,p1,p2,pos,period);
    m_letters.back().CreatePathShape(pos, 1, 1, Randv(period1,period2));
    //m_letters.back().CreatePath();
    //return;
    dx = 0.15f;
    pos.x += dx;
    Make_H(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 1, 1, Randv(period1, period2));
   
   // m_letters.back().CreatePathShape(pos, 0, 1, period);
    //m_letters.back().CreatePath();
    //pos.x = -0.5;
    //Make_H(pos, 0.25, dInc);
    
    //return;
    pos.x += dx;
    Make_Y(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 1, Randv(period1, period2));
    pos.x += dx;
    Make_S(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 1, 1, Randv(period1, period2));
    pos.x += dx;
    Make_I(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 1, Randv(period1, period2));
    pos.x += dx;
    Make_C(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 1, 1, Randv(period1, period2));
    pos.x += dx;
    Make_S(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 1, Randv(period1, period2));
   
    pos.y = -0.2f;// 30.0f;
    pos.x = -0.8f; // 7; // -150.0;
    Make_S(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 0, Randv(period1, period2));
    m_letters.back().CreatePath();
    pos.x += dx;
    Make_I(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 1, 0, Randv(period1, period2));
    pos.x += dx;
    Make_M(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 0, Randv(period1, period2));
    pos.x += dx;
    Make_U(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 1, 0, Randv(period1, period2));
    pos.x += dx;
    Make_L(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 0, Randv(period1, period2));
    pos.x += dx;
    Make_A(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 1, 0, Randv(period1, period2));
    pos.x += dx;
    Make_T(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 0, Randv(period1, period2));
    pos.x += dx;
    Make_I(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 1, 0, Randv(period1, period2));
    pos.x += dx;
    Make_O(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 0, Randv(period1, period2));
    pos.x += dx;
    Make_N(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 1, 0, Randv(period1, period2));
    pos.x += dx;
    Make_S(pos, scale, dInc);
    m_letters.back().CreatePathShape(pos, 0, 0, Randv(period1, period2));

    return;
    while (pos.x < 160) {
        Make_P(pos,scale,dInc);
        pos.x += dx;
    }

    pos.y = 0.0f;
    pos.x = -150.0f;
    while (pos.x < 160) {
        Make_P(pos,scale, dInc);
        pos.x += dx;
    }
}

void Words::ScaleToScreen(glm::vec2& pt)
{
    pt *= glm::vec2(win_w / 2.0f, win_h / 2.0f);
}

void Words::Update(float dt) {

    drops.clear(); 
    for (auto& let : m_letters)
    {        
        let.Update(dt);
        let.GetLetter(drops);
    }
    /*
    m_P.Update(dt);
    m_P.GetLetter(drops);
    m_P2.Update(dt);
    m_P2.GetLetter(drops);
    */
    if (!drops.size()) {
        return;
    }
    
    glm::vec2 force(16.0, 0.0);

    float x = 10.0f, y = 10.0f, delta = 5.0f;
    glm::vec2 vel(10.2f, 20.5f);
    int count = 0;
    int val = sizeof(Particle2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    size_t bytes_needed = sizeof(Particle2) * drops.size();
    if (bytes_needed > bytes_allocated) {
        glBufferData(GL_ARRAY_BUFFER, bytes_needed, (const float*)glm::value_ptr(drops[0].position), GL_STREAM_DRAW);
        bytes_allocated = bytes_needed;
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, bytes_needed, (const float*)glm::value_ptr(drops[0].position));
    }
}

void Words::Draw() {
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindVertexArray(vao);
    glUseProgram(m_shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, drops.size()); 
    // Don't forget to reset to default blending mode
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Words::AddDrop(glm::vec2 position, float mass) {
    if (mass < 0.01f) {
        mass = 0.01f;
    }

    Particle2 drop;
    drop.mass = mass;
    drop.inv_mass = 1.0f / mass;
    drop.position = position;
    drop.size = 10.0f; // Random(2, 20);
    drop.velocity = glm::vec2(0.0f, 0.0f);
    drop.forces = glm::vec2(0.0f, 0.0f);
    drop.life = 5.0f;
    //drop.lifeOn = 1;
    drops.push_back(drop);
    int i = 0;
    i++;
}

GLuint Words::LoadTexture2(const char* imagepath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
    GLuint textureID;

    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // Give the image to OpenGL
    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    }
    else {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    stbi_image_free(data);
    return textureID;
}

void Words::Make_P(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;    
    //Make the backwards 'C' part
    //circle equation 2*pi*r ... r*cos(d), r*sin(d)
    glm::vec2 offset(0.0, 0.0); // (((float)win_w) / 4.0, ((float)win_h) / 4.0);
    //float scale = 50.0f;
    float r = 0.25f;
    float angle = 3.14f / 2.0f;
    float pidiv2 = 3.14f / 2.0f;
    float dAngle = dInc * 5.0f; // 0.025f;
    float dx = -0.2f, dy = 0.25f; //offset, i.e. location of center of circle
    float x = -0.2f;
    float y = 0.5f;
    p.SetPosition(pos.x, pos.y);
    float dx2 = 0.2f;
    float range = 10.0f;
    while(angle > -pidiv2)
    {
        Particle2 wd;
        wd.size = m_size;
        wd.position.x = (r * cos(angle) + dx)*scale + dx2*scale;
        wd.position.y = (r * sin(angle) + dy)*scale;
        wd.velocity.x = Randv(-m_range1, m_range1);
        wd.velocity.y = Randv(m_range1, m_range2);
        wd.offset.x = 0; //Random(-range, range);
        wd.offset.y = 0; // Random(-range, range);
        wd.life = Randv(0.5, 3.0);
        wd.resetLife = wd.life;
        p.Add(wd);
        angle -= dAngle;
    }

    VerticalLine(p, glm::vec2(-0.2f, -0.5f), 0.5,scale, dInc);
    HorizontalLine(p, glm::vec2(-0.2, 0.5), 0.0, scale, dInc);
    HorizontalLine(p, glm::vec2(-0.2, 0.0), 0.0, scale, dInc);
    p.DuplicateLetter();
    p.SetScreen(win_w, win_h); 
    
    //p.SetVelocity(30.0f, -30.0f);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));

    m_letters.push_back(p);
    return;
    //Make the horizontal line 'l' part
    x = -0.2f;
    y = 0.5f;
    dy = 0.05f;
    while ( y > -0.5f)
    {
        Particle2 wd;
        wd.size = 2.0f;
        wd.position.x = x*scale + offset.x;
        wd.position.y = y*scale + offset.y;
        p.Add(wd);
        y -= dy;
    }
   
    return;
    /*
    //Experimenting
    x = 0.5;
    y = 0.5;
    while (y > -0.5f)
    {
        Particle2 wd;
        wd.size = 10.0f;
        wd.position.x = x * scale + offset.x;
        wd.position.y = y * scale + offset.y;
        p.Add(wd);
        y -= dy;
    }

    x = -0.5;
    y = -.5;
    dy = 0.05;
    dx = 0.05;
    while (x < 0.5) 
    {
        Particle2 wd;
        wd.size = 10.0f;
        wd.position.x = x * scale + offset.x;
        wd.position.y = y * scale + offset.y;
        p.Add(wd);
        x += dx;
    }
    */
}

void Words::Make_H(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);
    VerticalLine(p, glm::vec2(-0.2f, 0.5f), -0.5f, scale, dInc);
    HorizontalLine(p, glm::vec2(-0.2f, 0.05f), 0.2f, scale, dInc);
    VerticalLine(p, glm::vec2(0.2f, 0.5f), -0.5f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_Y(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);
    DiagonalLine(p, glm::vec2(-0.2f, 0.5f), glm::vec2(0.0f, 0.0f), scale, dInc);
    DiagonalLine(p, glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.5f), scale, dInc);
    VerticalLine(p, glm::vec2(0.0f, 0.0f), -0.5f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_I(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);
    HorizontalLine(p, glm::vec2(-0.1f, 0.5f), 0.1f, scale, dInc);    
    VerticalLine(p, glm::vec2(0.0f, 0.5f), -0.5f, scale, dInc);
    HorizontalLine(p, glm::vec2(-0.1f, -0.5f), 0.1f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_M(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);
    DiagonalLine(p, glm::vec2(-0.2f, -0.5f), glm::vec2(-0.15f, 0.5f), scale, dInc/4.0f);
    DiagonalLine(p, glm::vec2(-0.15f, 0.5f), glm::vec2(0.0f, 0.0f), scale, dInc);
    DiagonalLine(p, glm::vec2(0.0f, 0.0f), glm::vec2(0.15f, 0.5f), scale, dInc);
    DiagonalLine(p, glm::vec2(0.15f, 0.5f), glm::vec2(0.2f, -0.5f), scale, dInc/4.0f);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_L(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);

    VerticalLine(p, glm::vec2(-0.2f, 0.5f), -0.5f, scale, dInc);
    HorizontalLine(p, glm::vec2(-0.2f, -0.5f), 0.2f, scale, dInc);    

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_A(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);

    DiagonalLine(p, glm::vec2(-0.2f, -0.5f), glm::vec2(-0.0f, 0.5f), scale, dInc);
    DiagonalLine(p, glm::vec2(0.0f, 0.5f), glm::vec2(0.2f, -0.5f), scale, dInc);
    HorizontalLine(p, glm::vec2(-0.1f, 0.0f), 0.1f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_T(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);

    VerticalLine(p, glm::vec2(0.0f, 0.5f), -0.5f, scale, dInc);
    HorizontalLine(p, glm::vec2(-0.2f, 0.5f), 0.2f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_N(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);

    VerticalLine(p, glm::vec2(-0.2f, 0.5f), -0.5f, scale, dInc);
    DiagonalLine(p, glm::vec2(-0.2f, 0.5f), glm::vec2(0.2f, -0.5f), scale, dInc);
    VerticalLine(p, glm::vec2(0.2f, 0.5f), -0.5f, scale, dInc);    

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_O(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);

    EllipseLine(p, glm::vec2(0.0f, 0.0f), 0.2f, 0.5f, 0.0f, 6.28f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_C(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);

    //EllipseLine(p, glm::vec2(0.0, 0.0), 0.2, 0.5, 1.2, 5.08, scale, dInc);
    //EllipseLine(p, glm::vec2(0.0, 0.0), 0.2, 0.5, 1.0, 5.28, scale, dInc);
    EllipseLine(p, glm::vec2(0.0f, 0.0f), 0.2f, 0.5f, 0.8f, 5.48f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_U(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);

    VerticalLine(p, glm::vec2(-0.2f, 0.5f), 0.0f, scale, dInc);    
    VerticalLine(p, glm::vec2(0.2f, 0.5f), -0.5, scale, dInc);
    EllipseLine(p, glm::vec2(0.0f, 0.0f), 0.2f, 0.5f, 3.14f, 6.28f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::Make_S(glm::vec2 pos, float scale, float dInc)
{
    LetterData p;
    p.SetPosition(pos.x, pos.y);
    float yp = 0.25f;

    EllipseLine(p, glm::vec2(0.0, yp), 0.2f, 0.25f, 0.8f, 4.71f, scale, dInc);
    EllipseLine(p, glm::vec2(0.0, -yp), 0.2f, 0.25f, 3.94f, 6.28f, scale, dInc);
    EllipseLine(p, glm::vec2(0.0, -yp), 0.2f, 0.25f, 0.0f, 1.57f, scale, dInc);

    p.DuplicateLetter();
    p.SetScreen(win_w, win_h);
    p.SetVelocity(Randv(-50.0f, 50.0f), Randv(-50.0f, 50.0f));
    m_letters.push_back(p);
}

void Words::HorizontalLine2(LetterData& p, glm::vec2 ptStart, float xEnd, float scale, float dInc)
{
    //Make the horizontal line 'l' part
    float x, y, dx;
    if (ptStart.x > xEnd)
    {
        x = ptStart.x;
        ptStart.x = xEnd;
        xEnd = x;
    }

    x = ptStart.x; // -0.5;
    y = ptStart.y; // -.5;  
    dx = dInc; // 0.005;
    float range = 10.0f;
    while (x < xEnd)
    {
        Particle2 wd;
        wd.size = m_size;
        wd.position.x = x * scale;
        wd.position.y = y * scale;
        wd.velocity.x = Randv(-m_rangeA, m_rangeA);
        wd.velocity.y = Randv(m_rangeA, m_rangeB);
        wd.offset.x = 0.0f; // Random(-range, range);
        wd.offset.y = 0.0f; // Random(-range, range);
        wd.life = Randv(0.5f, 3.0f);
        wd.resetLife = wd.life;
        p.Add(wd);
        x += dx;
    }
    return;
}

void Words::VerticalLine2(LetterData& p, glm::vec2 ptStart, float yEnd, float scale, float dInc)
{
    float x, y, dy;

    if (ptStart.y > yEnd)
    {
        y = ptStart.y;
        ptStart.y = yEnd;
        yEnd = y;
    }
    x = ptStart.x; // 0.5;
    y = ptStart.y; // 0.5;
    dy = dInc; // 0.005;
    float range = 10.0f;
    while (y < yEnd)
    {
        Particle2 wd;
        wd.size = m_size;
        wd.position.x = x * scale;
        wd.position.y = y * scale;
        wd.velocity.x = Randv(-m_rangeA, m_rangeA);
        wd.velocity.y = Randv(m_rangeA, m_rangeB);
        wd.offset.x = 0.0f; // Random(-range, range);
        wd.offset.y = 0.0f; // Random(-range, range);
        wd.life = Randv(0.5f, 3.0f);
        wd.resetLife = wd.life;
        p.Add(wd);
        y += dy;
    }
}

void Words::HorizontalLine(LetterData& p, glm::vec2 ptStart, float xEnd,float scale,float dInc)
{
    //Make the horizontal line 'l' part
    float x, y, dx;
    if (ptStart.x > xEnd)
    {
        x = ptStart.x;
        ptStart.x = xEnd;
        xEnd = x;
    }

    x = ptStart.x; // -0.5;
    y = ptStart.y; // -.5;  
    dx = dInc; // 0.005;
    float range = 10.0f;
    while (x < xEnd)
    {
        Particle2 wd;
        wd.size = m_size;
        wd.position.x = x * scale;
        wd.position.y = y * scale;
        wd.velocity.x = Randv(-m_range1, m_range1);
        wd.velocity.y = Randv(m_range1, m_range2);
        wd.offset.x = 0.0f; // Random(-range, range);
        wd.offset.y = 0.0f; // Random(-range, range);
        wd.life = Randv(0.5f, 3.0f);
        wd.resetLife = wd.life;
        p.Add(wd);
        x += dx;
    }
    return;
}

void Words::VerticalLine(LetterData& p, glm::vec2 ptStart, float yEnd, float scale, float dInc)
{
    float x, y, dy;

    if (ptStart.y > yEnd)
    {
        y = ptStart.y;
        ptStart.y = yEnd;
        yEnd = y;
    }
    x = ptStart.x; // 0.5;
    y = ptStart.y; // 0.5;
    dy = dInc; // 0.005;
    float range = 10.0f;
    while (y < yEnd)
    {
        Particle2 wd;
        wd.size = m_size;
        wd.position.x = x * scale;
        wd.position.y = y * scale;
        wd.velocity.x = Randv(-m_range1, m_range1);
        wd.velocity.y = Randv(m_range1, m_range2);
        wd.offset.x = 0.0f; // Random(-range, range);
        wd.offset.y = 0.0f; // Random(-range, range);
        wd.life = Randv(0.5f, 3.0f);
        wd.resetLife = wd.life;
        p.Add(wd);
        y += dy;
    }
}

void Words::DiagonalLine(LetterData& p, glm::vec2 ptStart, glm::vec2 ptEnd, float scale, float dInc)
{
    float x, y, C, dx=dInc/2.0f;
    float rise, run, m;
   
    if (ptStart.x > ptEnd.x)
    {
        glm::vec2 tmp;
        tmp = ptStart;
        ptStart = ptEnd;
        ptEnd = tmp;        
    }
    rise = ptEnd.y - ptStart.y;
    run = ptEnd.x - ptStart.x;
    //Get the y(0) intercept
    // y = rise/run * x + y(0)
    // y(0) = y - mx; //y*run/rise -x
    C = ptEnd.y - rise / run * ptEnd.x; // *run / rise - ptEnd.x;
    x = ptStart.x;
    m = rise / run;
    while (x < ptEnd.x)
    {
        Particle2 wd;
        y = m * x + C;
        wd.size = m_size;
        wd.position.x = x * scale;
        wd.position.y = y * scale;
        wd.velocity.x = Randv(-m_range1, m_range1);
        wd.velocity.y = Randv(m_range1, m_range2);
        wd.offset.x = 0.0f; // Random(-range, range);
        wd.offset.y = 0.0f; // Random(-range, range);
        wd.life = Randv(0.5f, 3.0f);
        wd.resetLife = wd.life;
        p.Add(wd);
        x += dx;
    }
    
}

void Words::EllipseLine(LetterData& p, glm::vec2 ptStart, float major, float minor, float angStart, float angEnd,float scale, float dInc)
{
    float x, y, dAng = dInc * 5.0f;// / 2.0f;
    float angle;
    if (angStart > angEnd) {
        float tmp;
        tmp = angStart;
        angStart = angEnd;
        angEnd = tmp;
    }


    angle = angStart;
    while (angle < angEnd)
    {
        Particle2 wd;

        x = major * glm::cos(angle) + ptStart.x;
        y = minor * glm::sin(angle) + ptStart.y;

        wd.size = m_size;
        wd.position.x = x * scale;
        wd.position.y = y * scale;
        wd.velocity.x = Randv(-m_range1, m_range1);
        wd.velocity.y = Randv(m_range1, m_range2);
        wd.offset.x = 0.0f; // Random(-range, range);
        wd.offset.y = 0.0f; // Random(-range, range);
        wd.life = Randv(0.5f, 3.0f);
        wd.resetLife = wd.life;
        p.Add(wd);
        angle += dAng;
    }
}

LetterData::LetterData() : m_position(0.0f, 0.0f),  m_velocity(0.0f, 0.0f)
{

}

void LetterData::Add(Particle2 wd)
{
    m_letter.push_back(wd);
}

void LetterData::Clear()
{
    m_letter.clear();
}

void LetterData::CreatePathShape(glm::vec2 endPt, int shape,int invert,float period)
{
    float inv = 1.0f;
    if (invert) {
        inv = -1.0f;
    }

    if (shape == 0) {
        m_pts[0] = endPt + inv * glm::vec2(0.1f, 1.1f);
        m_pts[1] = endPt + inv * glm::vec2(-1.0f, 0.8f);
        m_pts[2] = endPt + inv * glm::vec2(0.4f, 0.3f);
        m_pts[3] = endPt;
    }
    else if (shape == 1) {
        m_pts[0] = endPt + inv * glm::vec2(0.1f, 1.1f);
        m_pts[1] = endPt + inv * glm::vec2(0.4f, 0.8f);
        m_pts[2] = endPt + inv * glm::vec2(-1.0f, 0.3f);
        m_pts[3] = endPt;
    }
    
    m_period = period;
    m_timeRemaining = m_period;
}

void LetterData::CreatePath()
{
    float dt = 0.05f, t = 0.0;
    glm::vec2 pt;
    float size=5.0;


    for (t = 0.0f; t <= 1.0f; t += dt) {
        pt = BezierPath(m_pts[0], m_pts[1], m_pts[2], m_pts[3], t);
        ScaleToScreen(pt);
        Particle2 wd;
        wd.size = 5.0;
        wd.position = pt;
        if (t == 0) {
            wd.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//Black
        }
        else if (t >= 1.0f) {
            wd.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//White
        }
        else {
            wd.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);//Yellow
        }
        m_path.push_back(wd);
    }
    //Control points:
    Particle2 wd;
    wd.size = size;    
    wd.position = m_pts[0];
    ScaleToScreen(wd.position);
    wd.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //Black
    m_path.push_back(wd);

    wd.size = size;
    wd.position = m_pts[1];
    ScaleToScreen(wd.position);
    wd.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); //Red
    m_path.push_back(wd);

    wd.size = size;
    wd.position = m_pts[2];
    ScaleToScreen(wd.position);
    wd.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); //Green
    m_path.push_back(wd);

    wd.size = size;
    wd.position = m_pts[3];
    ScaleToScreen(wd.position);
    wd.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//White
    m_path.push_back(wd);

}

void LetterData::ScaleToScreen(glm::vec2& pt)
{
    pt *= glm::vec2(m_width / 2.0f, m_height / 2.0f);
}

void LetterData::Update(float deltaTime)
{
    m_transform = glm::mat4(1.0f);
    float halfWidth = m_width / 2.0f;
    float halfHeight = m_height / 2.0f;
    float deltaAngle = 0.01f;
    float t=1.0f;

    if (m_timeRemaining > 0.0f) {
        t = 1.0f - m_timeRemaining / m_period;
        m_position = BezierPath(m_pts[0], m_pts[1], m_pts[2], m_pts[3], t);
        m_timeRemaining -= deltaTime;
    }
    else {
        m_position = m_pts[3];        
    }

    //glm::vec2 deltaPos = m_velocity * deltaTime;
    //m_velocity.y = 0.0f;

    //m_position += m_velocity * deltaTime;
    /*
    if (m_position.y < -halfHeight) {
        m_velocity.y *= -1.0f;
    }
    if (m_position.y > halfHeight)
    {
        m_velocity.y *= -1.0f;
    }

    if (m_position.x < -halfWidth) {
        m_velocity.x *= -1.0f;
    }
    if (m_position.x > halfWidth)
    {
        m_velocity.x *= -1.0f;        
    }
    */

    float angY = sin(m_angle);
    float angX = cos(m_angle);
    m_angle += deltaAngle;
    if (m_angle > 6.28f)
        m_angle = 0.0f;

    float dv1 = 0.1f, dv2=0.1f;
    m_transform = glm::translate(m_transform, glm::vec3(m_position, 0.0f));
    int i = 0;
    float grn = 0.0f, tclr=0.1f;
    t = 1.0f - t;
    float d;
    for (auto v : m_base)
    {
        glm::vec4 val = m_transform * glm::vec4(v.position, 0.0f, 1.0f);

        d = t + 0.015f;
        m_letter[i].offset +=  m_letter[i].velocity * deltaTime*d;
        m_letter[i].life -= deltaTime;

        
        //m_letter[i].offset.x = 0.0f;  //Shut off effects for testing, 2/25/20, 9:27 p.m.
        //m_letter[i].offset.y = 0.0f;
       
        if (m_letter[i].life <= 0)
        {
            m_letter[i].offset.x = 0.0f;
            m_letter[i].offset.y = 0.0f;
            m_letter[i].life = m_letter[i].resetLife;
        }
        m_letter[i].position.x = val.x + m_letter[i].offset.x;
        m_letter[i].position.y = val.y + m_letter[i].offset.y;

        ScaleToScreen(m_letter[i].position);
        
        //Scale to screen

        grn = m_letter[i].life / m_letter[i].resetLife;
        if (grn < 0.3f) {
            int jj = 0;
            jj++;
        }        
        m_letter[i].color = glm::vec4(1.0f, grn, 0.0f, m_alpha); // angY* m_letter[i].offset.y, 0.0f);
        i++;
        if (i > 100)
            tclr = 1.0f;
        continue;
        angY += dv1;
        angX += dv2;
        if (angY > 1.0f) {
            dv1 = -0.1f;
            angY = 1.0f;
        }
        if (angY < -1.0f) {
            dv1 = 0.1f;
            angY = -1.0f;
        }
        if (angX > 1.0f) {
            dv2 = -0.1f;
            angX = 1.0f;
        }
        if (angX < -1.0f) {
            dv2 = 0.1f;
            angX = -1.0f;
        }
        i++;
    }
}

glm::vec2 LetterData::BezierPath(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t)
{
    glm::vec2 rslt;
    if (t < 0.0f)
        return p0;
        
    if (t > 1.0f)
        return p3;

    // Cubic Bezier function from http://algorithmist.net/docs/bezier3.pdf
    // B(t) = (1-t)^3*p0 + 3*t(1-t)^2 * p1 + 3*t^2(1-t)*p2 + t^3*p3
    //p0 is the starting point, i.e. t = 0.   p3 is the end point, i.e. t = 1.
    rslt = (1 - t) * (1 - t) * (1 - t) * p0;
    rslt += 3 * t * (1 - t) * (1 - t) * p1;
    rslt += 3 * t * t * (1 - t) * p2;
    rslt += t * t * t * p3;
    return rslt;
}