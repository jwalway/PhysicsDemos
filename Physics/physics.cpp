/////////////////////////////////////////////////////////////////////////////
// Name:        penguin.cpp
// Purpose:     wxGLCanvas demo program
// Author:      Robert Roebling
// Modified by: Sandro Sigala
// Created:     04/01/98
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
#error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#include "physics.h"
#ifdef __DARWIN__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

//#include "../../sample.xpm"
#include "sample.xpm"

// ---------------------------------------------------------------------------
// MyApp
// ---------------------------------------------------------------------------

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    // Create the main frame window
    MyFrame* frame = new MyFrame(NULL, wxT("wxWidgets Penguin Sample"),
        wxDefaultPosition, wxSize(400, 500));  // wxDefaultSize);



#if wxUSE_ZLIB
    if (wxFileExists(wxT("penguin.dxf.gz")))
        frame->GetCanvas()->LoadDXF(wxT("penguin.dxf.gz"));
#else
    if (wxFileExists(wxT("penguin.dxf")))
        frame->GetCanvas()->LoadDXF(wxT("penguin.dxf"));
#endif

    /* Show the frame */
    frame->Show(true);

    return true;
}

IMPLEMENT_APP(MyApp) //Creates a WinMain() function and an instance of our MyApp class

// ---------------------------------------------------------------------------
// MyFrame
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_OPEN, MyFrame::OnMenuFileOpen)
EVT_MENU(wxID_EXIT, MyFrame::OnMenuFileExit)
EVT_MENU(wxID_HELP, MyFrame::OnMenuHelpAbout)
// EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE()

// MyFrame constructor
MyFrame::MyFrame(wxFrame* frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
    SetIcon(wxICON(sample));

    // Make the "File" menu
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, wxT("&Open..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tALT-X"));
    // Make the "Help" menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_HELP, wxT("&About"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));
    SetMenuBar(menuBar);

    /*
    m_btn1 = new wxButton(this, 10001, "Click Me", wxPoint(10, 10), wxSize( 150, 50));
    m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 30));
    m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
    */

    /*
    btn = new wxButton * [nFieldWidth * nFieldHeight];
    wxGridSizer *grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

    nField = new int[nFieldWidth * nFieldHeight];  //For Mind Sweeper game.

    wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    for (int x = 0; x < nFieldWidth; x++)
    {
        for (int y = 0; y < nFieldHeight; y++)
        {
            btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
            btn[y * nFieldWidth + x]->SetFont(font);  //Set the font for each button
            grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);  // wxEXPAND = occupy all space of cell

            //This associates the OnButtonClicked() function with each button for a click event
            btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnButtonClicked, this);
            nField[y * nFieldWidth + x] = 0; //default value
        }
    }

    this->SetSizer(grid); //tells parent window which sizer to use.
    grid->Layout(); // Restructure yourself according to parent's dimensions and number of buttons.
    */

    //Create the main layout
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    m_framesizer = sizer;
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN); // wxSP_NOBORDER);
    //wxSizer* sizer = new wxFlexGridSizer(2, 5, 5);
    wxSizer* subsizer;

    m_list = new wxListCtrl(splitter, wxID_ANY, wxDefaultPosition, wxSize(200, 300),
        wxLC_REPORT | wxSUNKEN_BORDER);
    m_list->AppendColumn(wxT("Topics:"));

    //sizer->Add(m_list, 1, wxLEFT, 10);
    sizer->Add(splitter, 1, wxEXPAND);

    subsizer = new wxBoxSizer(wxVERTICAL);
    m_panel = new wxPanel(splitter, wxID_ANY);
    m_canvas = new TestGLCanvas(m_panel, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, // wxSize(250,300),
        wxSUNKEN_BORDER);

    subsizer->Add(m_canvas, 1, wxEXPAND, 5);
    //m_buttonOne->Bind(wxEVT_BUTTON, &MainFrameFunctions::buttonOneClicked, this);


    wxSizer* wrapsizer = new wxWrapSizer(wxHORIZONTAL); // , wxWRAPSIZER_DEFAULT_FLAGS);
    m_wrapsizer = wrapsizer;

    m_btn1 = new wxButton(m_panel, 10001, "Click Me", wxDefaultPosition, wxDefaultSize); // wxPoint(10, 10), wxSize(150, 50));
    m_txt1 = new wxTextCtrl(m_panel, 10002, "Text Goes Here", wxDefaultPosition, wxDefaultSize);

    m_btn1->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
    wrapsizer->Add(m_btn1);
    wrapsizer->Add(m_txt1);
    subsizer->Add(wrapsizer, 1, wxEXPAND, 5);
    m_panel->SetSizer(subsizer);

    splitter->SplitVertically(m_list, m_panel); // , 100);
   // splitter->SetSize(GetClientSize());
    splitter->SetMinimumPaneSize(20);

    //sizer->Add(subsizer, 1, wxEXPAND | wxBOTTOM, 5);
    this->SetSizer(sizer);
    Show(true);

    // m_canvas = new TestGLCanvas(this, wxID_ANY, wxDefaultPosition,
     //    GetClientSize(), wxSUNKEN_BORDER);
     //glGetIntegerv(GL_MAJOR_VERSION, &maj);
     //glGetIntegerv(GL_MINOR_VERSION, &min);
}


MyFrame::~MyFrame()
{
    //delete[] btn;
   // delete[] nField;
}

void MyFrame::OnButtonClicked(wxCommandEvent& evt)
{
    //m_list1->AppendString(m_txt1->GetValue());

    //int x = (evt.GetId() - 10000) % nFieldWidth;
    //int y = (evt.GetId() - 10000) / nFieldWidth;

    if (evt.GetId() == 10001)
    {
        if (m_wrapsizer->GetItemCount() > 1)
        {
            m_wrapsizer->Remove(1);
            delete m_txt1; // = new wxTextCtrl(m_panel, 10002, "Text Goes Here", wxDefaultPosition, wxDefaultSize);
        }
        else {
            m_txt1 = new wxTextCtrl(m_panel, 10002, "Text Goes Here", wxDefaultPosition, wxDefaultSize);
            m_wrapsizer->Add(m_txt1);
        }
        m_panel->InvalidateBestSize();
        m_panel->Layout();
        //m_btn1->Unbind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
        //m_btn1->Unbind()
    }
    evt.Skip(); // Tells the system that the event has been handled.
}

// File|Open... command
void MyFrame::OnMenuFileOpen(wxCommandEvent& WXUNUSED(event))
{
    wxString filename = wxFileSelector(wxT("Choose DXF Model"), wxT(""), wxT(""), wxT(""),
#if wxUSE_ZLIB
        wxT("DXF Drawing (*.dxf;*.dxf.gz)|*.dxf;*.dxf.gz|All files (*.*)|*.*"),
#else
        wxT("DXF Drawing (*.dxf)|*.dxf)|All files (*.*)|*.*"),
#endif
        wxFD_OPEN);
    if (!filename.IsEmpty())
    {
        m_canvas->LoadDXF(filename);
        m_canvas->Refresh(false);
    }
}

// File|Exit command
void MyFrame::OnMenuFileExit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

// Help|About command
void MyFrame::OnMenuHelpAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("OpenGL Penguin Sample (c) Robert Roebling, Sandro Sigala et al"));
}

// ---------------------------------------------------------------------------
// TestGLCanvas
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(TestGLCanvas, wxGLCanvas)
EVT_SIZE(TestGLCanvas::OnSize)
EVT_PAINT(TestGLCanvas::OnPaint)
EVT_ERASE_BACKGROUND(TestGLCanvas::OnEraseBackground)
EVT_MOUSE_EVENTS(TestGLCanvas::OnMouse)
wxEND_EVENT_TABLE()

TestGLCanvas::TestGLCanvas(wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name)
    : wxGLCanvas(parent, id, NULL, pos, size,
        style | wxFULL_REPAINT_ON_RESIZE, name)
{
    // Explicitly create a new rendering context instance for this canvas.
    m_glRC = new wxGLContext(this);

    m_gldata.initialized = false;

    // initialize view matrix
    m_gldata.beginx = 0.0f;
    m_gldata.beginy = 0.0f;
    m_gldata.zoom = 45.0f;
    trackball(m_gldata.quat, 0.0f, 0.0f, 0.0f, 0.0f);
}

TestGLCanvas::~TestGLCanvas()
{
    delete m_glRC;
}

void TestGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    // must always be here
    wxPaintDC dc(this);

    SetCurrent(*m_glRC);

    // Initialize OpenGL
    if (!m_gldata.initialized)
    {
        InitGL();
        ResetProjectionMode();
        m_gldata.initialized = true;
    }

    // Clear
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -20.0f);
    GLfloat m[4][4];
    build_rotmatrix(m, m_gldata.quat);
    glMultMatrixf(&m[0][0]);

    m_renderer.Render();

    // Flush
    glFlush();

    // Swap
    SwapBuffers();
}

void TestGLCanvas::OnSize(wxSizeEvent& WXUNUSED(event))
{
    // Reset the OpenGL view aspect.
    // This is OK only because there is only one canvas that uses the context.
    // See the cube sample for that case that multiple canvases are made current with one context.
    ResetProjectionMode();
}

void TestGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing on MSW
}

// Load the DXF file.  If the zlib support is compiled in wxWidgets,
// supports also the ".dxf.gz" gzip compressed files.
void TestGLCanvas::LoadDXF(const wxString& filename)
{
    wxFileInputStream stream(filename);
    if (stream.IsOk())
#if wxUSE_ZLIB
    {
        if (filename.Right(3).Lower() == wxT(".gz"))
        {
            wxZlibInputStream zstream(stream);
            m_renderer.Load(zstream);
        }
        else
        {
            m_renderer.Load(stream);
        }
    }
#else
    {
        m_renderer.Load(stream);
    }
#endif
}

void TestGLCanvas::OnMouse(wxMouseEvent& event)
{
    if (event.Dragging())
    {
        wxSize sz(GetClientSize());

        /* drag in progress, simulate trackball */
        float spin_quat[4];
        trackball(spin_quat,
            (2.0 * m_gldata.beginx - sz.x) / sz.x,
            (sz.y - 2.0 * m_gldata.beginy) / sz.y,
            (2.0 * event.GetX() - sz.x) / sz.x,
            (sz.y - 2.0 * event.GetY()) / sz.y);

        add_quats(spin_quat, m_gldata.quat, m_gldata.quat);

        /* orientation has changed, redraw mesh */
        Refresh(false);
    }

    m_gldata.beginx = event.GetX();
    m_gldata.beginy = event.GetY();
}

void TestGLCanvas::InitGL()
{
    static const GLfloat light0_pos[4] = { -50.0f, 50.0f, 0.0f, 0.0f };

    // white light
    static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

    static const GLfloat light1_pos[4] = { 50.0f, 50.0f, 0.0f, 0.0f };

    // cold blue light
    static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };

    /* remove back faces */
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* speedups */
    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

    /* light */
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    char* glver = (char*)glGetString(GL_VERSION);
    int i = 0;
    i++;
}

void TestGLCanvas::ResetProjectionMode()
{
    if (!IsShownOnScreen())
        return;

    // This is normally only necessary if there is more than one wxGLCanvas
    // or more than one wxGLContext in the application.
    SetCurrent(*m_glRC);

    int w, h;
    GetClientSize(&w, &h);

    // It's up to the application code to update the OpenGL viewport settings.
    // In order to avoid extensive context switching, consider doing this in
    // OnPaint() rather than here, though.
    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
