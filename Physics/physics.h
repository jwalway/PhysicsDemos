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

#if wxUSE_ZLIB
#include "wx/zstream.h"
#endif

#include "wx/glcanvas.h"
#include "wx/richtext/richtextctrl.h"

extern "C"
{
#include "trackball.h"
}

#include "dxfrenderer.h"


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
class TestGLCanvas;


class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame* frame, const wxString& title, const wxPoint& pos,
        const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

    void OnMenuFileOpen(wxCommandEvent& event);
    void OnMenuFileExit(wxCommandEvent& event);
    void OnMenuHelpAbout(wxCommandEvent& event);

    void SetCanvas(TestGLCanvas* canvas) { m_canvas = canvas; }
    TestGLCanvas* GetCanvas() { return m_canvas; }

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
    TestGLCanvas* m_canvas;

    wxDECLARE_EVENT_TABLE();
};


class TestGLCanvas : public wxGLCanvas
{
public:
    TestGLCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("TestGLCanvas"));

    virtual ~TestGLCanvas();

    void LoadDXF(const wxString& filename);

protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouse(wxMouseEvent& event);

private:
    void InitGL();
    void ResetProjectionMode();

    wxGLContext* m_glRC;
    GLData       m_gldata;
    DXFRenderer  m_renderer;

    wxDECLARE_NO_COPY_CLASS(TestGLCanvas);
    wxDECLARE_EVENT_TABLE();
};

#endif // #ifndef _PHYSICS_H_