#include "objectunit.h"
#include <sstream>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Initializes object for rendering
void ObjectUnit::InitObject()
{
    return;
    unsigned int VBO, VAO, EBO;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
   // glGenBuffers(1, &m_vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
  //  glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    // Give our vertices to OpenGL.
  //  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    float scale = 0.5f, z = 0.0f;
    float vertices[] = {
        // positions          // colors           // texture coords
         scale,  scale, z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         scale, -scale, z,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -scale, -scale, z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -scale,  scale, z,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    int theSize = sizeof(m_vertices);
    int theSize2 = sizeof(vertices);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VAO);
    glGenBuffers(1, &EBO);
    // ..:: Initialization code :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_verticesSize, m_vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesSize, m_indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    m_VAO = VAO;
}

void ObjectUnit::Draw(float deltaTime, unsigned int shaderProgram)
{
    return;
    float dx = 0.0f, dy = 0.0f;
    float velx = 0.2f, vely = 0.2f;
    dx = m_velocity.x * deltaTime; // velx* deltaTime;
    dy = m_velocity.y * deltaTime; // vely* deltaTime;
    m_position.x += dx;
    m_position.y += dy;

    if (m_position.x > 1.0f)
    {
        m_position.x = 1.0f;
        m_velocity.x = m_velocity.x * -1.0f;
    }
    if (m_position.x < -1.0f)
    {
        m_position.x = -1.0f;
        m_velocity.x = m_velocity.x * -1.0f;
    }

    if (m_position.y > 1.0f)
    {
        m_position.y = 1.0f;
        m_velocity.y = m_velocity.y * -1.0f;
    }
    if (m_position.y < -1.0f)
    {
        m_position.y = -1.0f;
        m_velocity.y = m_velocity.y * -1.0f;
    }
    /*
    //SetCurrent(GetContext());
    //SimulationGLCanvas& canvas = wxGetApp().GetContext
   // wxGLCanvas::SetCurrent(*GetContext());
    static float xdist = 0.0f, ydist = 0.0f;
    static float signx = 1, signy = 1;

    xdist += (float)dx * signx;
    ydist += (float)dy * signy;

    if (xdist > 1.0f) {
        xdist = 1.0f;
        signx = -1;
    }
    if (xdist < -1.0f) {
        xdist = -1.0f;
        signx = 1;
    }
    if (ydist > 1.0f) {
        ydist = 1.0f;
        signy = -1.0f;
    }
    if (ydist < -1.0f) {
        ydist = -1.0f;
        signy = 1.0f;
    }


    glm::vec4 vec(5.0f, 5.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    static float deg = 0.0f;
    deg += 0.3f;
    if (deg >= 360.0f)
        deg = 0.0f;
    trans = glm::translate(trans, glm::vec3(xdist, ydist, 0.0f));
    trans = glm::rotate(trans, glm::radians(deg), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    vec = trans * vec;

    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    glUseProgram(m_shaderProgram);

    */

    glm::vec4 vec(5.0f, 5.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    static float deg = 0.0f;
    deg += 0.3f;
    if (deg >= 360.0f)
        deg = 0.0f;
    //trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));
    trans = glm::translate(trans, glm::vec3(m_position.x, m_position.y, m_position.z));
    trans = glm::rotate(trans, glm::radians(deg), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));    

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int ObjectUnit::LoadObject(deque<string> &objectData)
{   
    string elm;
    stringstream strData;
    vector<float> nums;
    vector<int> idxs;
    while(!objectData.empty())
    {
        elm = objectData.front();
        objectData.pop_front();  
 
        if (elm == "<texture1>")
        {
            m_textureFile1 = objectData.front();
            m_texture1 = LoadTexture(m_textureFile1.c_str());
            objectData.pop_front();            
            objectData.pop_front(); //Assume "</texture1>" exists and move to next element
        }
        if (elm == "<texture2>")
        {
            m_textureFile2 = objectData.front();
            m_texture2 = LoadTexture(m_textureFile2.c_str());
            objectData.pop_front();
            objectData.pop_front(); //Assume "</texture2>" exists and move to next element 
            return 0;
        }
        else if (elm == "<vertices>")
        {
            nums.clear();
            while(objectData.front() != "</vertices>")
            {
                strData.str(""); strData.clear();
                strData << objectData.front();
                float value;
                while (strData >> value)
                {
                    nums.push_back(value);
                }
                objectData.pop_front();
            }
            objectData.pop_front();
            //OpenGL requires an array
            m_vertices = new float[nums.size()];
            for (int i = 0; i < nums.size(); i++)
                m_vertices[i] = nums[i];
            m_verticesSize = (int)nums.size()* sizeof(float);
        }    
        else if (elm == "<indices>")
        {
            idxs.clear();
            while (objectData.front() != "</indices>")
            {
                strData.str(""); strData.clear();
                strData << objectData.front();
                int value;
                while (strData >> value)
                {
                    idxs.push_back(value);
                }
                objectData.pop_front();
            }
            objectData.pop_front();
            //OpenGL requires an array
            m_indices = new int[idxs.size()];
            for (int i = 0; i < idxs.size(); i++)
                m_indices[i] = idxs[i];
            m_indicesSize = (int)idxs.size() * sizeof(float);
        }       
        else if (elm == "<position>")
        {
            strData.str(""); strData.clear();
            strData << objectData.front();
            objectData.pop_front();
            float value;
            int idx=0;
            while (strData >> value)
            {
                m_position[idx++] = value;
            }
            objectData.pop_front(); // assume "</position>" exists and move to next element
        }      
        else if (elm == "<velocity>")
        {
            strData.str(""); strData.clear();
            strData << objectData.front();
            objectData.pop_front();
            float value;
            int idx = 0;
            while (strData >> value)
            {
                m_velocity[idx++] = value;
            }
            objectData.pop_front(); // assume "</velocity>" exists and move to next element
        }     
    }
  return 1;
}

GLuint ObjectUnit::LoadTexture(const char* imagepath)
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

ObjectUnit::~ObjectUnit()
{
    if (m_vertices != nullptr)
        delete m_vertices;
    if (m_indices != nullptr)
        delete m_indices;

    unsigned int texs[2] = { m_texture1, m_texture2 };
    glDeleteTextures(2, texs);
}