#include "objectunit.h"
#include <sstream>
#include <time.h> 

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Initializes object for rendering
void ObjectUnit::InitObject()
{   
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
    glGenVertexArrays(1, &VAO);
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
    m_VBO = VBO;
    m_EBO = EBO;
}

/*
void ObjectUnit::Calculate(float deltaTime)
{
    float dx = 0.0f, dy = 0.0f;
    float velx = 0.2f, vely = 0.2f;


    //Spring Force F = kx, or F= k * delta_d
    //I'll be applying a second order differential to determine the new position of the ball
    //Now using the gravitational equation (F = G m1*m2/r^2)
    float GMM = 0.25f;
    float Force;
    //The point where the ball is supposed to be attracted. This can be seen as the location of a planet:
    glm::vec3 pointOfAttraction(0.0f, 0.0f, 0.0f);
    pointOfAttraction = m_gravityWell;
    //The direction and distance of the force between the ball and "planet":
    glm::vec3 vf = pointOfAttraction - m_position;
    float len = glm::length(vf);
    // if (len > 0.05f) {
    if (len < 0.05f)
        len = 0.05f;
    Force = GMM / (len * len); //(F = G m1 * m2 / r ^ 2)
    vf = glm::normalize(vf);
    //The resultant delta velocity of the ball due to the gravity force:
    vf = vf * Force * deltaTime;
    //Add the delta velocity from the gravitational force
    m_velocity += vf;

    //Calculate the delta positions
    dx = m_velocity.x * deltaTime; // velx* deltaTime;
    dy = m_velocity.y * deltaTime; // vely* deltaTime;

    // Add the delta positions
    m_position.x += dx;
    m_position.y += dy;

    // }

    float rightWall = 1.0f, leftWall = -1.0f, topWall = 1.0f, bottomWall = -1.0f;
    float deltaWall = 0.1f;

    rightWall -= deltaWall;
    leftWall += deltaWall;
    topWall -= deltaWall;
    bottomWall += deltaWall;



    if (m_position.x > rightWall)
    {
        m_position.x = rightWall;
        m_velocity.x = m_velocity.x * -1.0f;
    }
    if (m_position.x < leftWall)
    {
        m_position.x = leftWall;
        m_velocity.x = m_velocity.x * -1.0f;
    }

    if (m_position.y > topWall)
    {
        m_position.y = topWall;
        m_velocity.y = m_velocity.y * -1.0f;
    }
    if (m_position.y < bottomWall)
    {
        m_position.y = bottomWall;
        m_velocity.y = m_velocity.y * -1.0f;
    }

    glm::vec4 vec(5.0f, 5.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    static float deg = 0.0f;
    // deg += 0.3f;
    if (deg >= 360.0f)
        deg = 0.0f;
    //trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));
    trans = glm::translate(trans, glm::vec3(m_position.x, m_position.y, m_position.z));
    trans = glm::rotate(trans, glm::radians(deg), glm::vec3(0.0, 0.0, 1.0));
    //trans = glm::scale(trans, glm::vec3(0.25, 0.32, 1.0));    
    trans = glm::scale(trans, glm::vec3(0.25 * 0.5, 0.32 * 0.5, 1.0));
    m_trans = trans;
}
*/

void ObjectUnit::Calculate(float deltaTime)
{
    float dx = 0.0f, dy = 0.0f;
    float velx = 0.2f, vely = 0.2f;
    bool inflate = true;

    // m_position = m_gravityWell;
    if (deltaTime > 0.0125f) {
        deltaTime = 0.00125f;
    }
    //Spring Force F = kx, or F= k * delta_d
    //I'll be applying a second order differential to determine the new position of the ball
    //Now using the gravitational equation (F = G m1*m2/r^2)
    float GMM = 0.06f; // 0.025f;//0.125f;
    float Force;
    //The point where the ball is supposed to be attracted. This can be seen as the location of a planet:
    glm::vec3 pointOfAttraction(0.0f, 0.0f, 0.0f);
    pointOfAttraction = m_gravityWell;
    //The direction and distance of the force between the ball and "planet":
    glm::vec3 vf = pointOfAttraction - m_position;
    float len = glm::length(vf);
    if (len > 0.04f) {
        inflate = false;
        //if (len < 0.05f)
            len = 0.05f;
        Force = GMM / (len * len); //(F = G m1 * m2 / r ^ 2)
        vf = glm::normalize(vf);
        //The resultant delta velocity of the ball due to the gravity force:
        vf = vf * Force * deltaTime;
        //Add the delta velocity from the gravitational force
        m_velocity += vf;
        //Subtract off some drag for each iteration 1:53 p.m. 1/21/20
        //Calculate the delta positions
        float dragValue = 0.1f, dragX, dragY;
        dragX = dragValue * m_velocity.x * deltaTime;
        dragY = dragValue * m_velocity.y * deltaTime;
        m_velocity.x -= dragX;
        m_velocity.y -= dragY;
        dx = m_velocity.x * deltaTime; // velx* deltaTime;
        dy = m_velocity.y * deltaTime; // vely* deltaTime;

        // Add the delta positions
        m_position.x += dx;
        m_position.y += dy;

    }

    float wallSize = 2.0f;
    float rightWall = wallSize, leftWall = -wallSize, topWall = wallSize, bottomWall = -wallSize;
    float deltaWall = 0.1f;


    rightWall -= deltaWall;
    leftWall += deltaWall;
    topWall -= deltaWall;
    bottomWall += deltaWall;

    if (m_position.x > rightWall)
    {
        m_position.x = rightWall;
        m_velocity.x = m_velocity.x * -1.0f;
    }
    if (m_position.x < leftWall)
    {
        m_position.x = leftWall;
        m_velocity.x = m_velocity.x * -1.0f;
    }

    if (m_position.y > topWall)
    {
        m_position.y = topWall;
        m_velocity.y = m_velocity.y * -1.0f;
    }
    if (m_position.y < bottomWall)
    {
        m_position.y = bottomWall;
        m_velocity.y = m_velocity.y * -1.0f;
    }

    glm::vec4 vec(5.0f, 5.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    static float deg = 0.0f;
    float factor = 0.1f;
    // deg += 0.3f;
    if (deg >= 360.0f)
        deg = 0.0f;
    //trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));
    trans = glm::translate(trans, glm::vec3(m_position.x, m_position.y, m_position.z));
    trans = glm::rotate(trans, glm::radians(deg), glm::vec3(0.0, 0.0, 1.0));
    
    if (inflate) {
        if (m_inflateValue < 1.0f) {
            m_inflateValue += deltaTime * factor;
        }
        trans = glm::scale(trans, glm::vec3(0.25*m_inflateValue, 0.32*m_inflateValue, 1.0));    
    }
    else {
        trans = glm::scale(trans, glm::vec3(0.25 * 0.5, 0.32 * 0.5, 1.0));
    }
    
    m_trans = trans;
}


void ObjectUnit::Draw(float deltaTime, unsigned int shaderProgram)
{  
    Calculate(deltaTime);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_trans));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture1);
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, m_texture2);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//v is the vector to be given a random value.  And the range of values
// is startRange to EndRange
void RandomVec(glm::vec3& v,float startRange=-1.0f,float endRange=1.0f)
{
    if (startRange >= endRange) {
        startRange = -1.0f;
        endRange = 1.0f;
    }

    float scale = (endRange - startRange) / (float)RAND_MAX;

    v.x = scale * (float)rand();
    v.y = scale * (float)rand();
    v.z = 0.0f;
}

int ObjectUnit::LoadObject(deque<string> &objectData)
{   
    string elm;
    stringstream strData;
    vector<float> nums;
    vector<int> idxs;   
    //return 0;
    srand(time(NULL));
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
            RandomVec(m_position);
            m_initPosition = m_position;
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
            RandomVec(m_velocity);
            m_initVelocity = m_velocity;
            objectData.pop_front(); // assume "</velocity>" exists and move to next element
        }
        else if (elm == "<gravitywell>")
        {
            static int count = 0;
            static float posx = -0.75f, posy = 0.5f;
            strData.str(""); strData.clear();
            strData << objectData.front();
            objectData.pop_front();
            float value;
            int idx = 0;
            while (strData >> value)
            {
                m_gravityWell[idx++] = value;
            }
            
            count++;
            if (count > 7) {
                count = 1;
                posx = -0.75f;
                posy -= 0.5f;
            }
          //  m_gravityWell[0] = posx;
         //   posx += 0.20f; // 0.15f; // 0.25f;
          //  m_gravityWell[1] = posy;
            
            objectData.pop_front(); 
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

void ObjectUnit::Replay()
{
    m_inflateValue = 0.5f;
    m_velocity = m_initVelocity;
    m_position = m_initPosition;
}

ObjectUnit::~ObjectUnit()
{
    if (m_vertices != nullptr)
        delete m_vertices;
    if (m_indices != nullptr)
        delete m_indices;  
    int val = (int)glGetError();
    unsigned int texs[2] = { m_texture1, m_texture2 };
    glDeleteTextures(2, texs);
    val = (int)glGetError();
    glDeleteVertexArrays(1, &m_VAO);
    val = (int)glGetError();
    glDeleteBuffers(1, &m_VBO);
    val = (int)glGetError();
    glDeleteBuffers(1, &m_EBO);
    val = (int)glGetError();
    int i = 0;
    i++;
}