#include "animations.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
//#include "wx/defs.h"
//#include "wx/app.h"
//#include "wx/wx.h"

void trim(string &str, string trimchars = " \t\f\v\n\r")
{
    //const string whitespace = " \t\f\v\n\r";
    //string test = "    test1 test2 test3     \n\n";
    int start = (int)str.find_first_not_of(trimchars);
    int end = (int)str.find_last_not_of(trimchars);
    str.erase(0, start);
    str.erase((end - start) + 1);    
}

int AnimationSceneBase::LoadObjects(char* filename)
{
    //Parsing xml *.data file for the animation scene
    string dataElement;
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
            objUnit->InitObject();

        }
        else {
            objectData.push_back(elm);
        }
    }


    return 0;
}

void AnimationSceneBase::Draw(float deltaTime, unsigned int shaderProgram)
{
    for (auto& x : m_objects)
    {
        x->Draw(deltaTime, shaderProgram);
    }
}

AnimationSceneBase::~AnimationSceneBase()
{
    for (auto& x : m_objects)
    {
        delete x;
    }
}

/*

int AnimationScene::LoadObjects(char* filename)
{
	//Parsing xml *.data file for the animation scene
    string dataElement;
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
     
    for (string &elm : dataElements)
    {
        if (elm== "<object>")
        {
            //Start of an object
            objectData.clear();
            objectData.push_back(elm);
        }
        else if (elm.substr(0,4) == "<!--")
        { // skip comments
            continue;
        }
        else if (elm == "</object>")
        {
            objectData.push_back(elm);            
            //create object and send data to it
            ObjectUnit *objUnit = new ObjectUnit();
            //ObjectBase* objBase = objUnit;
            objUnit->LoadObject(objectData);
            m_objects.push_back(objUnit);
            objUnit->InitObject();

        }
        else {
            objectData.push_back(elm);            
        }
    }    
    

	return 0;
}

void AnimationScene::Draw(float deltaTime, unsigned int shaderProgram)
{
    for (auto& x : m_objects)
    {
        x->Draw(deltaTime, shaderProgram);
    }
}

*/