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

float positiveDelta(float x1, float x2)
{
    if (x1 < x2)
        return x2 - x1;
    return x1 - x2;
}

void AnimationScene::Process(float deltaTime)
{
    /*
    float ballDiameter = 0.1f;
    vector<glm::vec3> positions;
    vector<glm::vec3> velocities;
    for (auto& x : m_objects) {
        //1) Calculate the change of position and velocity for each project ignoring collisions
        x->Calculate(deltaTime);
        
    }
    //checking for all collisions is an O(n^2) process..  I'll use fast math check.
    //2) Check for collisions.
    int j = 1;
    for (int i = 0; i < m_objects.size() - 1; i++)
    {
        j = i + 1;
        ObjectUnit* ou1 = (ObjectUnit*)m_objects[i];
        for (; j < m_objects.size(); j++) {
            float dx, dy;            
            ObjectUnit* ou2 = (ObjectUnit *)m_objects[j];
            dx = positiveDelta(ou1->getPosition().x, ou2->getPosition().x);
            dy = positiveDelta(ou1->getPosition().y, ou2->getPosition().y);
            if (dx <= ballDiameter && dy <= ballDiameter)
            {
                //I'll call this a collision, just to lower processing requirements
            }
        }
    }
    
    //3) If there is a collisions recalibrate the velocities.
    */
}