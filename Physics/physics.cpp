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
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "splashscene.h"
#include "collisionscene.h"

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
EVT_LIST_ITEM_SELECTED((int)Splash::List, MyFrame::OnSelectSubject)
EVT_MENU(wxID_OPEN, MyFrame::OnMenuFileOpen)
EVT_MENU(wxID_EXIT, MyFrame::OnMenuFileExit)
EVT_MENU(wxID_HELP, MyFrame::OnMenuHelpAbout)
// EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE()



// MyFrame constructor
MyFrame::MyFrame(wxFrame* frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style) : wxFrame(frame, wxID_ANY, title, pos, size, style)
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
   

    m_list = new wxListCtrl(splitter3, (int)Splash::List, wxDefaultPosition, wxSize(200, 200),
        wxLC_REPORT | wxSUNKEN_BORDER);
    m_list->AppendColumn(wxT("Topics:"), wxLIST_FORMAT_LEFT, 200);    

    m_richTextCtrl = new wxRichTextCtrl(splitter3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL/*|wxWANTS_CHARS*/);
  
    sizer->Add(splitter, 1, wxEXPAND);

    m_panel = new wxPanel(splitter2, wxID_ANY);
    m_canvas = new SimulationGLCanvas(splitter2, wxID_ANY, wxDefaultPosition,
        wxDefaultSize,wxSUNKEN_BORDER);

    //m_buttonOne->Bind(wxEVT_BUTTON, &MainFrameFunctions::buttonOneClicked, this);
    
    //m_wrapsizer = new wxWrapSizer(wxHORIZONTAL);
    m_flexSizer = new wxFlexGridSizer(4);

    
    m_panel->SetSizer(m_flexSizer);
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
    //WriteInitialText();
    m_canvas->CreateInitialScene(m_animationScene);

    m_list->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    SplashPanel();
    shared_ptr<AnimationSceneBase> sp;
    sp = m_animationScene.lock();
    sp->Description(*m_richTextCtrl);
     //glGetIntegerv(GL_MAJOR_VERSION, &maj);
     //glGetIntegerv(GL_MINOR_VERSION, &min);
    //AnimationScene animate;
    //animate.LoadObjects("animation1.data");
}

void MyFrame::OnSelectItem(wxListEvent& event)
{
    int i = 0;
    i++;
}

void MyFrame::PopulateListBox()
{
    vector<string> topics = { "Introduction", "Uniform Acceleration", "Acceleration Due To Gravity", "Newton's First Law" };

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

/*
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
*/

MyFrame::~MyFrame()
{
    //delete[] btn;
   // delete[] nField;
}

void MyFrame::OnButtonClicked(wxCommandEvent& evt)
{
    //m_list1->AppendString(m_txt1->GetValue());

    m_canvas->Replay();
    /*
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
    */
    /*
    if (evt.GetId() == 10001)
    {
        if ( m_flexSizer->GetItemCount() == 4)
        {
            m_flexSizer->Remove(3);
            delete m_txt1; // = new wxTextCtrl(m_panel, 10002, "Text Goes Here", wxDefaultPosition, wxDefaultSize);
        }
        else {
            m_txt1 = new wxTextCtrl(m_panel, 10002, "Text Goes Here", wxDefaultPosition, wxDefaultSize);
            m_flexSizer->Add(m_txt1);
        }
        m_panel->InvalidateBestSize();
        m_panel->Layout();
        //m_btn1->Unbind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
        //m_btn1->Unbind()
    }
    */
    evt.Skip(); // Tells the system that the event has been handled.
}

void MyFrame::OnButton2Clicked(wxCommandEvent& evt)
{

    evt.Skip();
}

// Item selected from ListBoxCtrl
void MyFrame::OnSelectSubject(wxListEvent& event)
{    
    int index = event.m_itemIndex;    

    if (m_canvas->LoadScene(index, m_animationScene) == false)
        return;
    switch (index) {
    case 0:
        SplashPanel();
       //m_animationScene
        break;
    case 1:
        CollisionPanel();
        break;
    }
    shared_ptr<AnimationSceneBase> sp;
    sp = m_animationScene.lock();
    sp->Description(*m_richTextCtrl);
}

void MyFrame::SplashPanel()
{
    //Don't forget to unbind controls
    while (m_flexSizer->GetItemCount()) //m_flexSizer->IsEmpty() == false)
    {
        m_flexSizer->Remove(0);
    }
    //Delete controls from previous panel
    map<string, wxControl*>::iterator p;
    for (p = m_controls.begin(); p != m_controls.end(); p++)
    {
        if (p->second != nullptr) {
            delete p->second;
        }
    }   

    m_controls.clear();
    
    //Create a sizer for this panel
    wxFlexGridSizer *flexSizer = new wxFlexGridSizer(2,5,5);
    m_panel->SetSizer(flexSizer);   
    m_flexSizer = flexSizer;
    wxButton *butn1 = new wxButton(m_panel, 10001, "Run", wxDefaultPosition, wxDefaultSize);
    m_controls["run_button"] = butn1;
    butn1->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
    wxTextCtrl *txt1 = new wxTextCtrl(m_panel, 10002, "Text Goes Here", wxDefaultPosition, wxDefaultSize);
    m_controls["text"] = txt1;
    butn1->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);    
    m_flexSizer->Add(butn1); // , 0, 0, 5);
    m_flexSizer->Add(txt1);    
    m_panel->InvalidateBestSize();
    m_panel->Layout();  
    m_canvas->SetPanelControls(m_controls);
}

void MyFrame::CollisionPanel()
{
    //Don't forget to unbind controls
    while (m_flexSizer->GetItemCount())
    {
        m_flexSizer->Remove(0);
    }
    //Delete controls from previous panel
    map<string, wxControl*>::iterator p;
    for (p = m_controls.begin(); p != m_controls.end(); p++)
    {
        if (p->second != nullptr) {
            delete p->second;
        }
    }
    m_controls.clear();
    int idx = 10001;
    //Create a sizer for this panel
    wxFlexGridSizer* flexSizer = new wxFlexGridSizer(5,5,5);
    m_panel->SetSizer(flexSizer);
    m_flexSizer = flexSizer;

    //First Line
    wxButton* butn1 = new wxButton(m_panel, idx++, "Run", wxDefaultPosition, wxDefaultSize);
    m_controls["run_button"] = butn1;
    butn1->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClicked, this);
    m_flexSizer->Add(butn1);
    wxStaticText *stxt = new wxStaticText(m_panel, idx++, "Before Collision:");
    m_controls["spc1"] = stxt;
    m_flexSizer->Add(stxt);
    stxt = new wxStaticText(m_panel, idx++, " ----- ");
    m_controls["spc2"] = stxt;
    m_flexSizer->Add(stxt);
    stxt = new wxStaticText(m_panel, idx++, " ----- ");
    m_controls["spc3"] = stxt;
    flexSizer->Add(stxt);
    stxt = new wxStaticText(m_panel, idx++, " ----- ");
    m_controls["spc4"] = stxt;
    flexSizer->Add(stxt);

    //Second Line
    stxt = new wxStaticText(m_panel, idx++, "Mass #1 kg:");    
    m_flexSizer->Add(stxt);
    m_controls["mass1_txt"] = stxt;
    wxTextCtrl* txt1 = new wxTextCtrl(m_panel, idx++, "10.0", wxDefaultPosition, wxDefaultSize);  
    m_flexSizer->Add(txt1);
    m_controls["mass1"] = txt1;
    stxt = new wxStaticText(m_panel, idx++, "Velocity #1 m/s:");
    m_flexSizer->Add(stxt);
    m_controls["velocity1_txt"] = stxt;
    txt1 = new wxTextCtrl(m_panel, idx++, "20.0", wxDefaultPosition, wxDefaultSize);
    m_flexSizer->Add(txt1);
    m_controls["in_velocity1"] = txt1;
    
    wxChoice* choice = new wxChoice(m_panel, idx++); // , wxDefaultPosition, wxDefaultSize, ;
    choice->Append("Elastic");
    choice->Append("0.7");
    choice->Append("0.5");
    choice->Append("0.2");
    choice->Append("Inelastic");
    choice->SetSelection(0);
    m_flexSizer->Add(choice);
    m_controls["restitution"] = choice;
    //Third Line
    stxt = new wxStaticText(m_panel, idx++, "Mass #2 kg:");
    m_flexSizer->Add(stxt);
    m_controls["mass2_txt"] = stxt;
    txt1 = new wxTextCtrl(m_panel, 10002, "25.0", wxDefaultPosition, wxDefaultSize);
    m_flexSizer->Add(txt1);
    m_controls["mass2"] = txt1;
    stxt = new wxStaticText(m_panel, idx++, "Velocity #2 m/s:");
    m_flexSizer->Add(stxt);
    m_controls["velocity2_txt"] = stxt;
    txt1 = new wxTextCtrl(m_panel, 10002, "15.0", wxDefaultPosition, wxDefaultSize);
    m_flexSizer->Add(txt1);
    m_controls["in_velocity2"] = txt1;
    choice = new wxChoice(m_panel, idx++);
    choice->Append("Metric Units");
    choice->Append("English Units");
    choice->SetSelection(0);
    m_flexSizer->Add(choice);
    m_controls["units"] = choice;

    //Fourth Line
    stxt = new wxStaticText(m_panel, idx++, "After Collision:");
    m_flexSizer->Add(stxt);
    m_controls["spc5"] = stxt;
    stxt = new wxStaticText(m_panel, idx++, "Velocity 1?");
    m_flexSizer->Add(stxt);
    m_controls["out_velocity1"] = stxt;
    stxt = new wxStaticText(m_panel, idx++, "Velocity 2?");
    flexSizer->Add(stxt);
    m_controls["out_velocity2"] = stxt;

   
    m_panel->InvalidateBestSize();
    m_panel->Layout();
    m_canvas->SetPanelControls(m_controls);
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

bool SimulationGLCanvas::LoadScene(int sceneNumber, weak_ptr<AnimationSceneBase>& scene)
{
    //Note: I'm also going to have to take care of changing the front panels... 1/25/20, 8:42 p.m.
    //The front panel could be changed in void MyFrame::OnSelectSubject(wxListEvent& event) after
    //returning from this function (to confirm this function was successful.
    if (m_currentScene == sceneNumber)
        return false;
    
    if (sceneNumber == 0) {
        //SplashScene
        m_animationScene = make_shared<SplashScene>();
        m_animationScene->LoadObjects("..\\resources\\SplashScene\\animation1.data");
        m_animationScene->Initialize(1);
        scene = m_animationScene;
        m_currentScene = sceneNumber;
    }
    else if (sceneNumber == 1) {
        //collision screen
        //m_animationScene = make_unique<SplashScene>();
        //m_animationScene->LoadObjects("..\\resources\\SplashScene\\animationTest.data");
        //m_animationScene->Initialize();
        m_animationScene = make_shared<CollisionScene>();
        m_animationScene->LoadObjects("..\\resources\\CollisionScene\\collision.data");
        m_animationScene->Initialize();
        scene = m_animationScene;
        m_currentScene = sceneNumber;
       // m_animationScene = new CollisionScene();
    }
    return true;
}


void SimulationGLCanvas::CreateInitialScene(weak_ptr<AnimationSceneBase>& scene) {
    m_animationScene = make_shared<SplashScene>();
    scene = m_animationScene;
}

GLuint SimulationGLCanvas::LoadTexture(const char* imagepath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
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
    return textureID;
}

GLuint SimulationGLCanvas::LoadTextureBMP(const char* imagepath)
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

void SimulationGLCanvas::LoadShaders(const char* vertexFile, const char* fragmentFile)
{
    unsigned int vshader = CompileShader(vertexFile, GL_VERTEX_SHADER);
    if (!vshader) return;
    unsigned int fshader = CompileShader(fragmentFile, GL_FRAGMENT_SHADER);
    if (!fshader) return;
    unsigned int shaderProgram = LinkShaders(vshader, fshader);
    if (!shaderProgram) return;
    m_shaderProgram = shaderProgram;
}

// Pass the filename of the shader to be compiled, and the shader type, which could be
// either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
unsigned int SimulationGLCanvas::CompileShader(const char* filename, unsigned int shaderType)
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

unsigned int SimulationGLCanvas::LinkShaders(unsigned int vertex, unsigned int fragment)
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
    return m_shaderProgram;
}

SimulationGLCanvas::~SimulationGLCanvas()
{
    delete m_glRC;  //This causes a 0x502 error (Invalid Operation), 1/25/20, 5:23 p.m.
    int val = (int)glGetError();
    //if (m_animationScene != nullptr)
     //   delete m_animationScene;
    unsigned int txts[2] = { m_texture1, m_texture2 }; 
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

    SetCurrent(*m_glRC);

    // Initialize OpenGL
    if (!m_gldata.initialized)
    {
        InitGL();
        //ResetProjectionMode();
        ResetOrthoMode();
        m_gldata.initialized = true;
    }

    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -20.0f);
        
    m_animationScene->Draw(m_deltaSeconds);    
    
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

    Refresh(false);
    event.RequestMore();
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
    //m_texture1 = LoadTexture("bricks_256by256.bmp");
    //m_texture2 = LoadTexture("TestAlphaChannel.png");
    //m_texture2 = LoadTexture("grass.png");
    //m_texture2 = LoadTexture("Tylyn444.bmp");
}

void SimulationGLCanvas::InitGLScene()
{
    //Somewhere I will need to have a place where a new scene is selected and instantiated 1/19/20, 10:58 p.m.
 

    //m_animationScene = make_unique<SplashScene>();
    m_animationScene->LoadObjects("..\\resources\\SplashScene\\animation1.data");
    //m_animationScene->LoadObjects("..\\resources\\SplashScene\\animationTest.data");
    m_animationScene->Initialize(); 
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
