// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Chunk.h"

glm::ivec3 Chunk::size = { 32,64,32 };

Chunk::Chunk(const Chunk& other)
{
	//if (map != nullptr)
		//delete[] map, map = nullptr;
	if (other.map != nullptr)
	{
		map = new GLubyte[size.x * size.y * size.z * 3];
		for (int i = 0; i < size.x * size.y * size.z * 3; i++)
			map[i] = other.map[i];
	}
	x = other.x;
	z = other.z;
	std::cout << "ASSIGNMENT CHUNK OPERATOR CALLED";
}

Chunk::Chunk()
{
	std::cout << "BASUC CHUNK CONSTRUCTOR CALLED\n";
	map = nullptr;
}



Chunk::Chunk(int x, int z) :x(x), z(z)
{
	map = new GLubyte[size.x * size.y * size.z * 3];
}

Chunk::Chunk(int x, int z, WorldGenerator* wg) : x(x), z(z)
{
	map = new GLubyte[size.x * size.y * size.z * 3];
	wg->fillMap(glm::vec3(x * size.x, 0 * size.y, z * size.z), map, size);
	//std::cout << "USUAL CHUNK CONSTRUCTOR CALLED\n";
}

Chunk::~Chunk()
{
	if (map != nullptr) 
	{
		//delete[] map;
	}

}

Chunk& Chunk::operator=(const Chunk& ch)
{
	if (map != nullptr)
		delete[] map, map = nullptr;
	if (ch.map != nullptr)
	{
		map = new GLubyte[size.x * size.y * size.z * 3];
		for (int i = 0; i < size.x * size.y * size.z * 3; i++)
			map[i] = ch.map[i];
	}
	x = ch.x;
	z = ch.z;
	return *this;
}

glm::ivec3 Chunk::getSize()
{
	return size;
}

GLubyte* Chunk::getMap()
{
	return map;
}

void Chunk::setMap(GLubyte* m)
{
	delete[] map;
	map = m;
}

void Chunk::buildMesh(GLubyte* m, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth)
{
    std::cout << "BUILDING " << p0.x << " " << std::endl;
    //if (p0.x >= width)
        //std::cout << "ERROR " << p0.x << std::endl;
    //std::this_thread::sleep_for(2000);
    for (int x = p0.x; x < p1.x; x++)
        for (int y = p0.y; y < p1.y; y++)
            for (int z = p0.z; z < p1.z; z++)
            {
                int k = z * width * height + y * width + x;
                glm::ivec2 cube = glm::ivec2(m[k * gf], m[k * gf + 1]);
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
                    bool sides[6]{ 1,1,1,1,1,1 };
                    if (z > 0)
                        if (m[(k - width * height) * gf] != 0)
                            sides[0] = false;
                    if (sides[0])
                    {
                        vertices.resize(verticesSize + 36);
                        std::copy(front, front + 36, vertices.begin() + verticesSize);
                        verticesSize += 36;
                    }
                    if (y > 0)
                        if (m[(k - width) * gf] != 0)
                            sides[1] = false;
                    if (sides[1])
                    {
                        vertices.resize(verticesSize + 36);
                        std::copy(down, down + 36, vertices.begin() + verticesSize);
                        verticesSize += 36;
                    }
                    if (x > 0)
                        if (m[(k - 1) * gf] != 0)
                            sides[2] = false;
                    if (sides[2])
                    {
                        vertices.resize(verticesSize + 36);
                        std::copy(right, right + 36, vertices.begin() + verticesSize);
                        verticesSize += 36;
                    }
                    if (z < depth - 1)
                        if (m[(k + width * height) * gf] != 0)
                            sides[3] = false;
                    if (sides[3])
                    {
                        vertices.resize(verticesSize + 36);
                        std::copy(back, back + 36, vertices.begin() + verticesSize);
                        verticesSize += 36;
                    }
                    if (y < height - 1)
                        if (m[(k + width) * gf] != 0)
                            sides[4] = false;
                    if (sides[4])
                    {
                        vertices.resize(verticesSize + 36);
                        std::copy(up, up + 36, vertices.begin() + verticesSize);
                        verticesSize += 36;
                    }
                    if (x < width - 1)
                        if (m[(k + 1) * gf] != 0)
                            sides[5] = false;
                    if (sides[5])
                    {
                        vertices.resize(verticesSize + 36);
                        std::copy(left, left + 36, vertices.begin() + verticesSize);
                        verticesSize += 36;
                    }
                }
            }
    verticesToDraw = verticesSize / 6;

    loadingStatus = 0;
}

void Chunk::function(GLubyte* m, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth)
{
    std::vector<GLuint> verts;
    GLuint vertsSize = 0;
    for (int x = p0.x; x < p1.x; x++)
        for (int y = p0.y; y < p1.y; y++)
            for (int z = p0.z; z < p1.z; z++)
            {
                int k = z * width * height + y * width + x;
                GLuint a = m[k*2];
                glm::ivec2 cube = glm::ivec2(m[k * 2], m[k * 2 + 1]);
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
                    bool sides[6]{ 1,1,1,1,1,1 };
                    if (z > 0)
                        if (m[(k - width * height) * gf] != 0)
                            sides[0] = false;
                    if (sides[0])
                    {
                        verts.resize(vertsSize + 36);
                        //std::copy(front, front + 36, verts.begin() + vertsSize);
                        vertsSize += 36;
                    }
                }
            }
    verticesToDraw = verticesSize / 6;

    loadingStatus = 0;
}

void Chunk::prepareRender(GLubyte* m, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth)
{
    loadingStatus = -1;
    verticesSize = 0;
    verticesToDraw = 0;
    vertices.clear();
    //v.resize(width * height * depth * 2);
    //std::copy(m, m + width * height * depth * 2, v.begin());
    buildMesh(m, p0, p1, width, height, depth);
    //std::thread thread(&Chunk::function, this, std::ref(m), p0, p1, width, height, depth);
    //thread.join();
    //thread.detach();
    //std::cout << verticesToDraw << "\n";


}

void Chunk::loadMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    loadingStatus = 1;
}

void Chunk::moveMesh(glm::ivec2 delta)
{
    for (int i = 0; i < verticesToDraw; i++)
    {
        vertices[i * 6] += delta.x;
        vertices[i * 6 + 2] += delta.y;
    }
    loadingStatus = 0;
}

void Chunk::clearMesh()
{
    vertices.clear();
    loadingStatus = -1;
}

void Chunk::render()
{
    glBindVertexArray(VAO);


    //glDisable(GL_DEPTH_TEST);
    //std::cout << verticesSize << " " << verticesToDraw << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);





    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Chunk::endRender()
{
}

glm::ivec3 Chunk::getCube(glm::ivec3 pos)
{
	int i = size.y * size.x * pos.z + size.x * pos.y + pos.x;
		return glm::ivec3(map[i * 3], map[i * 3 + 1], map[i * 3 + 2]);
}

glm::ivec3 Chunk::getCube(int x0, int y0, int z0)
{
	int i = size.y * size.x * z0 + size.x * y0 + x0;
	return glm::ivec3(map[i * 3], map[i * 3 + 1], map[i * 3 + 2]);
}

glm::ivec3 Chunk::getPosition()
{
	return glm::ivec3(x, 0, z);
}

int Chunk::getLoadingStatus()
{
    return loadingStatus;
}

void Chunk::stLoadingStatus(int ls)
{
    loadingStatus = ls;
}

void Chunk::placeCube(glm::ivec3 pos, glm::ivec3 cube)
{
	int i = size.y * size.x * pos.z + size.x * pos.y + pos.x;
	map[i * 3] = cube.x;
	map[i * 3 + 1] = cube.y;
	map[i * 3 + 2] = cube.z;
}