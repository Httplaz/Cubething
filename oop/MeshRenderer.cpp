#include "MeshRenderer.h"
#define gf 2
MeshRenderer::MeshRenderer()
{
    test = new GLubyte[4 * 1920 * 1080];
    for (int i = 0; i < 4 * 1920 * 1080; i++)
        test[i] = 128;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GL_LINEAR));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GL_LINEAR));
    int width = 1920, height = 1080;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    screenShader = Shader("vert.glsl", "frag.glsl");
    tshader = Shader("vert1.glsl", "frag1.glsl");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &EBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "FRAMEBUFFER COMPLETE";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

MeshRenderer::~MeshRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &FBO);
    glDeleteBuffers(1, &EBO);
}

void MeshRenderer::createScreenMesh()
{
    GLfloat vertices2[] =
    {
        // Позиции          // Текстурные координаты
        //1.,  -1., 0,      // Верхний правый
       // 1.,  1., 0,        // Нижний правый
        //-1.,  1., 0,       // Нижний левый
        //-1.,  -1., 0,      // Верхний левый


        1.,  -1., 0,      // Верхний правый
        -1.,  -1., 0,      // Верхний левый
        -1.,  1., 0,       // Нижний левый
        1.,  -1., 0,      // Верхний правый
        1.,  1., 0,        // Нижний правый
        -1.,  1., 0,       // Нижний левый
    };
    GLuint indices[] =
    {
        0, 3, 2,
        0, 1, 2
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);



    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    GLfloat verts[] =
    {
        0.5,0.5,0.0,
        -0.5, 0.5, 0.0,
        0.0,0.0, 0.0
    };



    //glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);

    glBindVertexArray(TVAO);
    glBindBuffer(GL_ARRAY_BUFFER, TVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //verticesToDraw = 6;
}

void MeshRenderer::prepareFrameBuffer()
{

}

void MeshRenderer::closeFrameBuffer()
{
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MeshRenderer::createWorldMesh(GLubyte* map, int width, int height, int depth)
{
    verticesSize = 0;

    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
            for (int z = 0; z < depth; z++)
            {
                int k = z * width * height + y * width + x;
                glm::ivec2 cube = glm::ivec2(map[k * gf], map[k * gf + 1]);
                if (cube.x > 0)
                {
                    //y = -y;
                    GLfloat front[] =
                    {
                        x,y,z, 0.f, 0,0,
                        x + 0.5,y,z, 0.f,1,0,
                        x + 0.5,y + 0.5,z, 0,1,1,

                        x + 0.5,y + 0.5,z,0,1,1,
                        x,y + 0.5, z, 0,0,1,
                        x,y,z, 0,0,0, //front
                    };

                    GLfloat back[] =
                    {
                        x + 0.5,y + 0.5,z + .5,3,0,1,
                        x + 0.5,y,z + 0.5,3,0,0,
                        x,y,z + 0.5, 3,1,0,

                        x,y,z + 0.5, 3,1,0,
                        x,y + 0.5, z + 0.5, 3,1,1,
                        x + 0.5,y + 0.5,z + 0.5,3,0,1, //back
                    };

                    GLfloat right[] =
                    {
                        x,y + 0.5,z,1,0,1,
                        x,y,z,1,0,0,
                        x,y,z + 0.5, 1,1,0,

                        x,y,z + 0.5, 1,1,0,
                        x,y + 0.5, z + 0.5, 1,1,1,
                        x,y + 0.5,z,1,0,1 //right
                    };

                    GLfloat up[] =
                    {
                        x,y + 0.5,z, 5,1,1,
                        x + 0.5,y + 0.5,z,5,1,0,
                        x + 0.5,y + 0.5,z + 0.5,5,0,0,

                        x + 0.5,y + 0.5,z + 0.5,5,0,0,
                        x,y + 0.5, z + 0.5, 5,0,1,
                        x,y + 0.5,z, 5,1,1, //up
                    };

                    GLfloat down[] =
                    {
                        x + 0.5,y,z + 0.5,2,0,1,
                        x + 0.5,y,z,2,1,1,
                        x,y,z, 2,1,0,

                        x,y,z, 2,1,0,
                        x,y, z + 0.5, 2,0,0,
                         x + 0.5,y,z + 0.5,2,0,1,//down
                    };

                    GLfloat left[] =
                    {
                         x + 0.5,y,z + 0.5, 4,0,0,
                        x + 0.5,y,z,4,1,0,
                        x + 0.5,y + 0.5,z,4,1,1,

                        x + 0.5,y + 0.5,z,4,1,1,
                        x + 0.5,y + 0.5, z + 0.5, 4,0,1,
                        x + 0.5,y,z + 0.5, 4, 0,0,//left
                    };

                    if (z > 0)
                        if (map[(k - width * height)*gf] == 0) 
                        {
                            vertices.resize(verticesSize + 36);
                            std::copy(front, front + 36, vertices.begin()+verticesSize);
                            verticesSize += 36;
                        }
                    if (y > 0)
                        if (map[(k - width)*gf] == 0)
                        {
                            vertices.resize(verticesSize + 36);
                            std::copy(down, down + 36, vertices.begin() + verticesSize);
                            verticesSize += 36;
                        }
                    if (x > 0)
                        if (map[(k - 1)*gf] == 0)
                        {
                            vertices.resize(verticesSize + 36);
                            std::copy(right, right + 36, vertices.begin() + verticesSize);
                            verticesSize += 36;
                        }
                    if (z < depth-1)
                        if (map[(k + width * height)*gf] == 0)
                        {
                            vertices.resize(verticesSize + 36);
                            std::copy(back, back + 36, vertices.begin() + verticesSize);
                            verticesSize += 36;
                        }
                    if (y < height-1)
                        if (map[(k + width)*gf] == 0)
                        {
                            vertices.resize(verticesSize + 36);
                            std::copy(up, up + 36, vertices.begin() + verticesSize);
                            verticesSize += 36;
                        }
                    if (x<width-1)
                        if (map[(k +1)*gf] == 0)
                        {
                            vertices.resize(verticesSize + 36);
                            std::copy(left, left + 36, vertices.begin() + verticesSize);
                            verticesSize += 36;
                        }
                }
            }
            verticesToDraw = verticesSize / 6;
            //for (int i = 0; i < verticesSize; i++)
                //std::cout << vertices[i] << " ";

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);


    //glEnable(GL_DEPTH_TEST);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    //std::cout << verticesToDraw << "\n";
}

void MeshRenderer::clearMesh()
{
    verticesSize = 0;
    vertices.clear();
}

void MeshRenderer::addWorldMesh(GLubyte* map, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth)
{
    int kp = p0.z * width * height + p0.y * width + p0.x;
    int mr0 = verticesSize;
    int addVerticesSize = 0;
    std::vector<GLfloat> addVertices;
    for (int x = p0.x; x < p1.x; x++)
        for (int y = p0.y; y < p1.y; y++)
            for (int z = p0.z; z < p1.z; z++)
            {
                int k = z * width * height + y * width + x;
                glm::ivec2 cube = glm::ivec2(map[k * gf], map[k * gf + 1]);
                if (cube.x > 0)
                {
                    //y = -y;
                    GLfloat front[] =
                    {
                        x,y,z, 0.f, 0,0,
                        x + 0.5,y,z, 0.f,1,0,
                        x + 0.5,y + .5,z, 0,1,1,

                        x + 0.5,y + 0.5,z,0,1,1,
                        x,y + 0.5, z, 0,0,1,
                        x,y,z, 0,0,0, //front
                    };

                    GLfloat back[] =
                    {
                        x + 0.5,y + 0.5,z + 0.5,3,0,1,
                        x + 0.5,y,z + 0.5,3,0,0,
                        x,y,z + 0.5, 3,1,0,

                        x,y,z + 0.5, 3,1,0,
                        x,y + 0.5, z + 0.5, 3,1,1,
                        x + 0.5,y + 0.5,z + 0.5,3,0,1, //back
                    };

                    GLfloat right[] =
                    {
                        x,y + 0.5,z,1,0,1,
                        x,y,z,1,0,0,
                        x,y,z + 0.5, 1,1,0,

                        x,y,z + 0.5, 1,1,0,
                        x,y + 0.5, z + 0.5, 1,1,1,
                        x,y + 0.5,z,1,0,1 //right
                    };

                    GLfloat up[] =
                    {
                        x,y + 0.5,z, 5,1,1,
                        x + 0.5,y + 0.5,z,5,1,0,
                        x + 0.5,y + 0.5,z + .5,5,0,0,

                        x + 0.5,y + 0.5,z + 0.5,5,0,0,
                        x,y + 0.5, z + 0.5, 5,0,1,
                        x,y + 0.5,z, 5,1,1, //up
                    };

                    GLfloat down[] =
                    {
                        x + 0.5,y,z + 0.5,2,0,1,
                        x + 0.5,y,z,2,1,1,
                        x,y,z, 2,1,0,

                        x,y,z, 2,1,0,
                        x,y, z + 0.5, 2,0,0,
                         x + 0.5,y,z + 0.5,2,0,1,//down
                    };

                    GLfloat left[] =
                    {
                         x + 0.5,y,z + 0.5, 4,0,0,
                        x + 0.5,y,z,4,1,0,
                        x + 0.5,y + 0.5,z,4,1,1,

                        x + 0.5,y + 0.5,z,4,1,1,
                        x + 0.5,y + 0.5, z + 0.5, 4,0,1,
                        x + 0.5,y,z + 0.5, 4, 0,0,//left
                    };

                    if (z > 0)
                        if (map[(k - width * height) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(front, front + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (y > 0)
                        if (map[(k - width) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(down, down + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (x > 0)
                        if (map[(k - 1) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(right, right + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (z < depth - 1)
                        if (map[(k + width * height) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(back, back + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (y < height - 1)
                        if (map[(k + width) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(up, up + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (x < width - 1)
                        if (map[(k + 1) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(left, left + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                }
            }
    vertices.insert(vertices.end(), addVertices.begin(), addVertices.end());
    verticesSize += addVerticesSize;
    int mr1 = verticesSize;
    meshRegions.insert(std::make_pair(kp, glm::ivec2(mr0, mr1)));
    verticesToDraw = verticesSize / 6;
    
}

void MeshRenderer::replaceWorldMesh(GLubyte* map, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth)
{
    int kp = p0.z * width * height + p0.y * width + p0.x;
    int mr0 = verticesSize;
    int addVerticesSize = 0;
    std::vector<GLfloat> addVertices;
    for (int x = p0.x; x < p1.x; x++)
        for (int y = p0.y; y < p1.y; y++)
            for (int z = p0.z; z < p1.z; z++)
            {
                int k = z * width * height + y * width + x;
                glm::ivec2 cube = glm::ivec2(map[k * gf], map[k * gf + 1]);
                if (cube.x > 0)
                {
                    //y = -y;
                    GLfloat front[] =
                    {
                        x,y,z, 0.f, 0,0,
                        x + 0.5,y,z, 0.f,1,0,
                        x + 0.5,y + 0.5,z, 0,1,1,

                        x + 0.5,y + 0.5,z,0,1,1,
                        x,y + 0.5, z, 0,0,1,
                        x,y,z, 0,0,0, //front
                    };

                    GLfloat back[] =
                    {
                        x + 0.5,y + 0.5,z + .5,3,0,1,
                        x + 0.5,y,z + 0.5,3,0,0,
                        x,y,z + 0.5, 3,1,0,

                        x,y,z + 0.5, 3,1,0,
                        x,y + 0.5, z + 0.5, 3,1,1,
                        x + 0.5,y + 0.5,z + 0.5,3,0,1, //back
                    };

                    GLfloat right[] =
                    {
                        x,y + 0.5,z,1,0,1,
                        x,y,z,1,0,0,
                        x,y,z + 0.5, 1,1,0,

                        x,y,z + 0.5, 1,1,0,
                        x,y + 0.5, z + 0.5, 1,1,1,
                        x,y + 0.5,z,1,0,1 //right
                    };

                    GLfloat up[] =
                    {
                        x,y + 0.5,z, 5,1,1,
                        x + 0.5,y + 0.5,z,5,1,0,
                        x + 0.5,y + 0.5,z + 0.5,5,0,0,

                        x + 0.5,y + 0.5,z + 0.5,5,0,0,
                        x,y + 0.5, z + 0.5, 5,0,1,
                        x,y + 0.5,z, 5,1,1, //up
                    };

                    GLfloat down[] =
                    {
                        x + 0.5,y,z + 0.5,2,0,1,
                        x + 0.5,y,z,2,1,1,
                        x,y,z, 2,1,0,

                        x,y,z, 2,1,0,
                        x,y, z + 0.5, 2,0,0,
                         x + 0.5,y,z + 0.5,2,0,1,//down
                    };

                    GLfloat left[] =
                    {
                         x + 0.5,y,z + 0.5, 4,0,0,
                        x + 0.5,y,z,4,1,0,
                        x + 0.5,y + 0.5,z,4,1,1,

                        x + 0.5,y + 0.5,z,4,1,1,
                        x + 0.5,y + 0.5, z + 0.5, 4,0,1,
                        x + 0.5,y,z + 0.5, 4, 0,0,//left
                    };

                    if (z > 0)
                        if (map[(k - width * height) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(front, front + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (y > 0)
                        if (map[(k - width) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(down, down + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (x > 0)
                        if (map[(k - 1) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(right, right + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (z < depth - 1)
                        if (map[(k + width * height) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(back, back + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (y < height - 1)
                        if (map[(k + width) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(up, up + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                    if (x < width - 1)
                        if (map[(k + 1) * gf] == 0)
                        {
                            addVertices.resize(addVerticesSize + 36);
                            std::copy(left, left + 36, addVertices.begin() + addVerticesSize);
                            addVerticesSize += 36;
                        }
                }
            }
   glm::ivec2 mr = meshRegions.find(kp)->second;

   vertices.erase(vertices.begin() + mr.x, vertices.begin() + mr.y);
   //std::cout << vertices[mr.x] << " ";
   vertices.insert(vertices.begin() + mr.x, addVertices.begin(), addVertices.end());
   //std::cout << addVertices[0] << " " << vertices[mr.x] << "\n";
   int diff = addVertices.size() - (mr.y - mr.x);
   meshRegions.find(kp)->second.x -= diff;
   for (auto it = meshRegions.find(kp); it != meshRegions.end(); it++)
       it->second.x += diff, it->second.y += diff;
   verticesSize = vertices.size();
   verticesToDraw = vertices.size() / 6;
}

void MeshRenderer::createMesh()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);


    //glEnable(GL_DEPTH_TEST);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void MeshRenderer::render()
{     
    glBindVertexArray(VAO);
    

    //glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);


  


    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

}

void MeshRenderer::postRender()
{
    glBindVertexArray(quadVAO);


    //glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);





    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

}

