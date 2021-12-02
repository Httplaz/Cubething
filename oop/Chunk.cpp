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
		map = new GLubyte[size.x * size.y * size.z * 2];
		for (int i = 0; i < size.x * size.y * size.z * 2; i++)
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
	map = new GLubyte[size.x * size.y * size.z * 2];
}

Chunk::Chunk(int x, int z, WorldGenerator* wg) : x(x), z(z)
{
	map = new GLubyte[size.x * size.y * size.z * 2];
	wg->fillMap(glm::vec3(x * size.x, 0 * size.y, z * size.z), map, size);
	//std::cout << "USUAL CHUNK CONSTRUCTOR CALLED\n";
}

Chunk::~Chunk()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

}

Chunk& Chunk::operator=(const Chunk& ch)
{
	if (map != nullptr)
		delete[] map, map = nullptr;
	if (ch.map != nullptr)
	{
		map = new GLubyte[size.x * size.y * size.z * 2];
		for (int i = 0; i < size.x * size.y * size.z * 2; i++)
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

void Chunk::setVertices(std::vector<GLfloat> vertices)
{
    if (this != nullptr) 
    {
        this->vertices = vertices;
        verticesSize = vertices.size();
        verticesToDraw = verticesSize / 6;
    }
}

std::vector<GLfloat> Chunk::buildMesh(GLubyte* m, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth, Chunk* chunk)
{
    //p0 += glm::ivec3(1);
    //p1 -= glm::ivec3(1);
    std::vector<GLfloat> verts;
    int vertsSize = 0;
    for (int x = p0.x; x < p1.x; x++)
        for (int y = p0.y; y < p1.y; y++)
            for (int z = p0.z; z < p1.z; z++)
            {
                int k = z * width * height + y * width + x;
                int rx = x % size.x;
                int ry = y % size.y;
                int rz = z % size.z;
                glm::ivec2 cube = glm::ivec3(0), cubeUp = glm::ivec3(0), cubeDown = glm::ivec3(0), cubeLeft = glm::ivec3(0), cubeRight = glm::ivec3(0), cubeFront = glm::ivec3(0), cubeBack = glm::ivec3(0);
                if (true)//(map == nullptr) 
                //if (map == nullptr) 
                {
                    cube = glm::ivec2(m[k * gf], m[k * gf + 1]);
                    if (x > 0)
                        cubeLeft = glm::ivec2(m[(k - 1) * gf], m[(k - 1) * gf + 1]);
                    if (y > 0)
                        cubeDown = glm::ivec2(m[(k - width) * gf], m[(k - width) * gf + 1]);
                    if (z > 0)
                        cubeFront = glm::ivec2(m[(k - width * height) * gf], m[(k - width * height) * gf + 1]);
                    if (x < width - 1)
                        cubeRight = glm::ivec2(m[(k + 1) * gf], m[(k + 1) * gf + 1]);
                    if (y < height-1)
                        cubeUp = glm::ivec2(m[(k + width) * gf], m[(k + width) * gf + 1]);
                    if (z < depth - 1)
                        cubeBack = glm::ivec2(m[(k + width * height) * gf], m[(k + width * height) * gf + 1]);
                }
                else
                {
                    cube = getCube(rx, ry, rz);
                    if (x > 0)
                        cubeLeft = getCube(rx - 1, ry, rz);
                    if (y > 0)
                        cubeDown = getCube(rx, ry-1, rz);
                    if (z > 0)
                        cubeFront = getCube(rx, ry, rz - 1);
                    if (x < size.x - 1)
                        cubeRight = getCube(rx + 1, ry, rz);
                    if (y < size.x - 1)
                        cubeUp = getCube(rx, ry + 1, rz);
                    if (z < size.z - 1)
                        cubeBack = getCube(rx, ry, rz + 1);
                }
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
                    if (cubeFront.x)
                        sides[0] = false;
                    if (sides[0])
                    {
                        verts.resize(vertsSize + 36);
                        std::copy(front, front + 36, verts.begin() + vertsSize);
                        vertsSize += 36;
                    }
                    if (cubeDown.x)
                        sides[1] = false;
                    if (sides[1])
                    {
                        verts.resize(vertsSize + 36);
                        std::copy(down, down + 36, verts.begin() + vertsSize);
                        vertsSize += 36;
                    }
                    if (cubeLeft.x)
                        sides[2] = false;
                    if (sides[2])
                    {
                        verts.resize(vertsSize + 36);
                        std::copy(right, right + 36, verts.begin() + vertsSize);
                        vertsSize += 36;
                    }
                    if (cubeBack.x)
                        sides[3] = false;
                    if (sides[3])
                    {
                        verts.resize(vertsSize + 36);
                        std::copy(back, back + 36, verts.begin() + vertsSize);
                        vertsSize += 36;
                    }
                    if (cubeUp.x)
                        sides[4] = false;
                    if (sides[4])
                    {
                        verts.resize(vertsSize + 36);
                        std::copy(up, up + 36, verts.begin() + vertsSize);
                        vertsSize += 36;
                    }
                    if (cubeRight.x)
                        sides[5] = false;
                    if (sides[5])
                    {
                        verts.resize(vertsSize + 36);
                        std::copy(left, left + 36, verts.begin() + vertsSize);
                        vertsSize += 36;
                    }
                }
            }
    verticesToDraw = verticesSize / 6;

    loadingStatus = 0;
    return verts;
}

void Chunk::prepareRender(GLubyte* m, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth)
{
    setVertices(buildMesh(m, p0, p1, width, height, depth,this));
}

void Chunk::loadMesh()
{
    if (!GLGenerated) 
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        GLGenerated = true;
    }

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

void Chunk::release()
{
    if (map != nullptr)
    {
        delete[] map;
        map = nullptr;
    }
}

glm::ivec3 Chunk::getCube(glm::ivec3 pos)
{
	int i = size.y * size.x * pos.z + size.x * pos.y + pos.x;
    return glm::ivec3(map[i * 2], map[i * 2 + 1], 0);// map[i * 2 + 2]);
}

glm::ivec3 Chunk::getCube(int x0, int y0, int z0)
{
    int i = size.y * size.x * z0 + size.x * y0 + x0;
    return glm::ivec3(map[i * 2], map[i * 2 + 1], 0);// map[i * 2 + 2]);
    return glm::ivec3(0);
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
	map[i * 2] = cube.x;
	map[i * 2 + 1] = cube.y;
	//map[i * 2 + 2] = cube.z;
}