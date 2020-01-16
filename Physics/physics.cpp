/////////////////////////////////////////////////////////////////////////////
// Name:        physics.cpp
// Purpose:     physics demo program
// Author:      John  Alway
// 
// Created:     1/13/2020
// Copyright:   (c) John Alway
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include <vector>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
#error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#include "zebra.xpm"
#include "physics.h"
#ifdef __DARWIN__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

//#include "../../sample.xpm"
#include "sample.xpm"

using namespace std;

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
        wxDefaultPosition, wxSize(800, 800));  // wxDefaultSize);



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
EVT_LIST_ITEM_SELECTED(wxID_ANY, MyFrame::OnSelectSubject)
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
    

    //Create the main layout
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    m_framesizer = sizer;
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);

    wxSplitterWindow* splitter2 = new wxSplitterWindow(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);

    wxSplitterWindow* splitter3 = new wxSplitterWindow(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);
   

    m_list = new wxListCtrl(splitter3, wxID_ANY, wxDefaultPosition, wxSize(200, 200),
        wxLC_REPORT | wxSUNKEN_BORDER);
    m_list->AppendColumn(wxT("Topics:"), wxLIST_FORMAT_LEFT, 200);

    m_richTextCtrl = new wxRichTextCtrl(splitter3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL/*|wxWANTS_CHARS*/);
  
    sizer->Add(splitter, 1, wxEXPAND);

    m_panel = new wxPanel(splitter2, wxID_ANY);
    m_canvas = new SimulationGLCanvas(splitter2, wxID_ANY, wxDefaultPosition,
        wxDefaultSize,wxSUNKEN_BORDER);

    //m_buttonOne->Bind(wxEVT_BUTTON, &MainFrameFunctions::buttonOneClicked, this);
    
    m_wrapsizer = new wxWrapSizer(wxHORIZONTAL);  

    m_btn1 = new wxButton(m_panel, 10001, "Click Me", wxDefaultPosition, wxDefaultSize); 
    m_txt1 = new wxTextCtrl(m_panel, 10002, "Text Goes Here", wxDefaultPosition, wxDefaultSize);

    m_btn1->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
    m_wrapsizer->Add(m_btn1);
    m_wrapsizer->Add(m_txt1);

    m_panel->SetSizer(m_wrapsizer);

    splitter2->SplitHorizontally(m_canvas, m_panel, 400);
    splitter2->SetMinimumPaneSize(150);
    splitter3->SplitHorizontally(m_list, m_richTextCtrl, 400);
    splitter3->SetMinimumPaneSize(100);


    splitter->SplitVertically(splitter2, splitter3, 550); 
    splitter->SetMinimumPaneSize(200);
    this->SetSizer(sizer);
    Show(true);

    PopulateListBox();
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    wxImage::AddHandler(new wxGIFHandler);
    WriteInitialText();
     //glGetIntegerv(GL_MAJOR_VERSION, &maj);
     //glGetIntegerv(GL_MINOR_VERSION, &min);

    LARGE_INTEGER Frequency, StartTime, EndTime, ElapsedTime;

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartTime);
    for (int k = 0; k < 1000; k++) {
        for (int j = 0; j < 1000; j++) {
            int l = 0;
            l++;
        }
    }
    QueryPerformanceCounter(&EndTime);
    ElapsedTime.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
    int stop = 0;
    stop++;
}

void MyFrame::PopulateListBox()
{
    vector<string> topics = { "Uniform Acceleration", "Acceleration Due To Gravity", "Newton's First Law" };

    for (int i = 0; i < topics.size(); i++)
        m_list->InsertItem(i, topics[i].c_str());
}

void MyFrame::WriteInitialText()
{
    wxRichTextCtrl& r = *m_richTextCtrl;

    //r.SetDefaultStyle();
    r.Freeze();
    r.BeginSuppressUndo();
    r.BeginParagraphSpacing(0, 20);
    r.BeginAlignment(wxTEXT_ALIGNMENT_CENTER);
    r.BeginBold();
    r.BeginFontSize(14);

    wxString lineBreak = (wxChar)29;

    r.WriteText(wxString(wxT("Welcome to wxRichTextCtrl, a wxWidgets control")) + lineBreak + wxT("for editing and presenting styled text and images\n"));
    r.EndFontSize();

    r.BeginItalic();
    r.WriteText(wxT("by John Alway"));
    r.EndItalic();
    r.EndBold();

    r.Newline();

    r.WriteImage(wxBitmap(zebra_xpm));

    r.Newline();
    r.Newline();

    r.EndAlignment();
    r.EndParagraphSpacing();
    r.EndSuppressUndo();
    r.Thaw();

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

// Item selected from ListBoxCtrl
void MyFrame::OnSelectSubject(wxListEvent& event)
{    
    int index = event.m_itemIndex;
 
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
// SimulationGLCanvas
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(SimulationGLCanvas, wxGLCanvas)
EVT_SIZE(SimulationGLCanvas::OnSize)
EVT_PAINT(SimulationGLCanvas::OnPaint)
EVT_ERASE_BACKGROUND(SimulationGLCanvas::OnEraseBackground)
EVT_MOUSE_EVENTS(SimulationGLCanvas::OnMouse)
EVT_IDLE(SimulationGLCanvas::OnIdle)
wxEND_EVENT_TABLE()

SimulationGLCanvas::SimulationGLCanvas(wxWindow* parent,
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
    QueryPerformanceFrequency(&m_frequency);  //Get the base frequence for timer
    QueryPerformanceCounter(&m_startTime); //Initialize counter
}

GLuint SimulationGLCanvas::LoadTexture(const char* imagepath)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char* data;

    // Open the file
    FILE* file = fopen(imagepath, "rb");
    if (!file) { printf("Image could not be opened\n"); return 0; }

    if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return textureID;
}

int SimulationGLCanvas::CompileLinkShaders()
{
    //First the Vertex Shader
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    //The first thing we need to do is create a shader object, again referenced by an ID
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Next we attach the shader source code to the shader object and compile the shader:
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Was the compile successful?
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    //Second the Fragment Shader
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

    //Compile the Fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //Was compile successful?
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    //Linking the two shaders

    //To use the recently compiled shaders we have to link them to a shader program object 
    //and then activate this shader program when rendering objects. The activated shader
    //program's shaders will be used when we issue render calls.

    //First create a program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    //Now attached the previously compiled shaders and link them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Was linking successful?

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        return -1;
    }
    m_shaderProgram = shaderProgram;
    return 0;
}

SimulationGLCanvas::~SimulationGLCanvas()
{
    delete m_glRC;
}

void SimulationGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    DrawScene();
}

void SimulationGLCanvas::OnSize(wxSizeEvent& WXUNUSED(event))
{
    // Reset the OpenGL view aspect.
    // This is OK only because there is only one canvas that uses the context.
    // See the cube sample for that case that multiple canvases are made current with one context.
    //ResetProjectionMode();
    ResetOrthoMode();
}

void SimulationGLCanvas::DrawScene()
{
    // must always be here
    wxPaintDC dc(this);
    double dx=0.0, dy=0.0;
    double velx = 1.0, vely = 1.0;
    dx = velx*m_deltaSeconds;
    dy = vely * m_deltaSeconds;
    //SetCurrent(GetContext());
    //SimulationGLCanvas& canvas = wxGetApp().GetContext
   // wxGLCanvas::SetCurrent(*GetContext());
    static float xdist=0.0f, ydist=0.0f;

    xdist += (float)dx;
    ydist += (float)dy;

    if (xdist > 10.0f)
        xdist = 0.0f;
    if (ydist > 10.0f)
        ydist = 0.0f;

    SetCurrent(*m_glRC);

    // Initialize OpenGL
    if (!m_gldata.initialized)
    {
        InitGL();
        //ResetProjectionMode();
        ResetOrthoMode();
        m_gldata.initialized = true;
    }
    static int count = 0;
    count++;

    // Clear
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -20.0f);
    GLfloat m[4][4];
    build_rotmatrix(m, m_gldata.quat);
    glMultMatrixf(&m[0][0]);


    //m_renderer.Render();

    // 1st attribute buffer : vertices
    /*
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
    */

    //Triangle using VBO... this code works! (1/15/20, 12:29 p.m.)
    // VBO transfers data right to the graphics card.
    /*
    float scale = 0.5f, z = 6.0f;

    static const GLfloat vertices[] = {
        0.0, scale, z,  // middle top corner
        -scale, -scale, z, // bottom left corner
         scale,  -scale, z // bottom right corner
    };

    //
    glEnableClientState(GL_VERTEX_ARRAY );
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);

     */
    float x = 1.0f, y = 1.0f;
    float z = 6.0f;
    x = x + xdist;
    y = y + ydist;
    /*
    glBegin(GL_QUADS);
    //glNormal3f(0.0f, 0.0f, z);
    glVertex3f(x, y, z);
    glVertex3f(-x, y, z);
    glVertex3f(-x, -y, z);
    glVertex3f(x, -y, z);
    glEnd();
     */

    //Texture tutorial

    // ..:: Drawing code (in render loop) :: ..
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


   // glBindTexture(GL_TEXTURE_2D, m_textureID);
    //glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 // Flush
    glFlush();

    // Swap
    SwapBuffers();
}

void SimulationGLCanvas::OnIdle(wxIdleEvent& event)
{
   // DrawScene();
    static int count = 0;
    count++;
    //event.RequestMore();
    QueryPerformanceCounter(&m_endTime);
    m_elapsedTime.QuadPart = m_endTime.QuadPart - m_startTime.QuadPart; //Number of ticks since last call
    m_startTime = m_endTime;
    m_elapsedTime.QuadPart *= 1000000; // set up for aseconds to microseconds conversion
    m_elapsedTime.QuadPart /= m_frequency.QuadPart; //Now determine the number of microseconds since last call of OnIdle()
    m_deltaSeconds = (double)m_elapsedTime.QuadPart / 1000000.0;

    //Refresh(false);
    //event.RequestMore();
}

void SimulationGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing on MSW
}

// Load the DXF file.  If the zlib support is compiled in wxWidgets,
// supports also the ".dxf.gz" gzip compressed files.
void SimulationGLCanvas::LoadDXF(const wxString& filename)
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

void SimulationGLCanvas::OnMouse(wxMouseEvent& event)
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

void SimulationGLCanvas::InitGL()
{
    static const GLfloat light0_pos[4] = { -50.0f, 50.0f, 0.0f, 0.0f };

    // white light
    static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

    static const GLfloat light1_pos[4] = { 50.0f, 50.0f, 0.0f, 0.0f };

    // cold blue light
    static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };

    /* remove back faces */
   // glEnable(GL_CULL_FACE);
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
    //glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    char* glver = (char*)glGetString(GL_VERSION);
    int i = 0;
    i++;
    GLenum res = glewInit();
    string str;
    if (res != GLEW_OK) {
        str = (char*)glewGetErrorString(res);
        wxLogFatalError("Error: '%s'\n", glewGetErrorString(res));
    }
    InitGLScene();
}

void SimulationGLCanvas::InitGLScene()
{
    float scale = 1.5f, z = 6.0f;
    // An array of 3 vectors which represents 3 vertices   
    static const GLfloat g_vertex_buffer_data[] = {
       -scale, -scale, z,
       scale, -scale, z,
       0.0f,  scale, z
    };
   
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &m_vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    //shape for texturing
    //unsigned int VBO;

  

    float vertices[] = {
    -scale, -scale, z,
     scale, -scale, z,
     0.0,  scale, z
    };
    
    unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
    };
    unsigned int VBO, VAO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VAO);
    glGenBuffers(1, &EBO);
// ..:: Initialization code :: ..
// 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_VAO = VAO;

    CompileLinkShaders();
}

void SimulationGLCanvas::ResetProjectionMode()
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

void SimulationGLCanvas::ResetOrthoMode()
{
    int w, h;

    if (!IsShownOnScreen())
        return;

    // This is normally only necessary if there is more than one wxGLCanvas
    // or more than one wxGLContext in the application.
    SetCurrent(*m_glRC);
       
    GetClientSize(&w, &h);
    
    // reset the viewport to the new dimensions, e.g. for screen resize
    glViewport(0, 0, (GLint)w, (GLint)h);


    // It's up to the application code to update the OpenGL viewport settings.
    // In order to avoid extensive context switching, consider doing this in
    // OnPaint() rather than here, though.
    //glViewport(0, 0, (GLint)w, (GLint)h);
    //gluOrtho2D(0, w, h, 0);

  

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
      
    // set up an orthographic projection with the same near clip plane
    //glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
    double mag = 0.02;
    glOrtho(-mag*(double)w, mag*(double)w, -mag*(double)h, mag*(double)h, 5, 100);
    //gluOrtho2D(0, w, h, 0);

    // select modelview matrix and clear it out
    glMatrixMode(GL_MODELVIEW);
}