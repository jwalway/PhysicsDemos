#pragma once
/////////////////////////////////////////////////////////////////////////////
// Name:        physics.cpp
// Purpose:     physics demo program
// Author:      John  Alway
// 
// Created:     1/13/2020
// Copyright:   (c) John Alway
/////////////////////////////////////////////////////////////////////////////

#ifndef _PHYSICS_H_
#define _PHYSICS_H_


#include "wx/defs.h"
#include "wx/app.h"
#include "wx/menu.h"
#include "wx/dcclient.h"
#include "wx/wfstream.h"
#include "wx/listctrl.h"
#include "wx/wrapsizer.h"
#include "wx/splitter.h"
#include <fstream>
#include <sstream>
#include "animations.h"

#if wxUSE_ZLIB
#include "wx/zstream.h"
#endif

#include "GL/glew.h"

#include "wx/glcanvas.h"
#include "wx/richtext/richtextctrl.h"

extern "C"
{
#include "trackball.h"
}

#include "dxfrenderer.h"
#define GL_GLEXT_PROTOTYPES
//#include "glext.h"



// OpenGL view data
struct GLData
{
    bool initialized;           // have OpenGL been initialized?
    float beginx, beginy;       // position of mouse
    float quat[4];              // orientation of object
    float zoom;                 // field of view in degrees
};


// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};


// Define a new frame type
class SimulationGLCanvas;


class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame* frame, const wxString& title, const wxPoint& pos,
        const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

    void OnMenuFileOpen(wxCommandEvent& event);
    void OnMenuFileExit(wxCommandEvent& event);
    void OnMenuHelpAbout(wxCommandEvent& event);

    void SetCanvas(SimulationGLCanvas* canvas) { m_canvas = canvas; }
    SimulationGLCanvas* GetCanvas() { return m_canvas; }

    ~MyFrame();
private:    
    wxButton* m_btn1 = nullptr;
    wxTextCtrl *m_txt1 = nullptr;
    wxListBox *m_list1 = nullptr;
    wxListCtrl *m_list = nullptr;
    wxPanel *m_panel = nullptr;
    wxRichTextCtrl *m_richTextCtrl = nullptr; 


    wxSizer* m_wrapsizer;
    wxSizer* m_framesizer;

    void OnButtonClicked(wxCommandEvent& evt);
    void OnSelectSubject(wxListEvent& event);

    void PopulateListBox();

    void WriteInitialText();
    
private:
    SimulationGLCanvas* m_canvas;

    wxDECLARE_EVENT_TABLE();
};


class SimulationGLCanvas : public wxGLCanvas
{
public:
    SimulationGLCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("SimulationGLCanvas"));

    virtual ~SimulationGLCanvas();

    void LoadDXF(const wxString& filename);
    void LoadShaders(const char* vertexFile,const char* fragmentFile); 

protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnIdle(wxIdleEvent& event);
    GLuint LoadTextureBMP(const char *imagepath);
    GLuint LoadTexture(const char *imagepath);

private:
    void InitGL();
    void ResetProjectionMode();
    void ResetOrthoMode();
    void InitGLScene();
    void InitGLScene2();
    void DrawScene();
    void DrawScene2();
    void DrawScene3();
    unsigned int CompileShader(const char* filename, unsigned int shaderType);
    unsigned int LinkShaders(unsigned int vertex, unsigned int fragment);

    wxGLContext* m_glRC;
    GLData       m_gldata;
    DXFRenderer  m_renderer; 
    AnimationScene* m_animationScene;
    GLuint m_vertexbuffer=0; // This will identify our vertex buffer
    LARGE_INTEGER m_frequency, m_startTime, m_endTime, m_elapsedTime;
    GLuint m_texture1, m_texture2;
    unsigned int m_VBO;
    unsigned int m_VAO;
    unsigned int m_shaderProgram;
    double m_deltaSeconds = 0.0;
    wxDECLARE_NO_COPY_CLASS(SimulationGLCanvas);
    wxDECLARE_EVENT_TABLE();
};


#endif // #ifndef _PHYSICS_H_
