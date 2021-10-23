#include "Chunk.h"

glm::ivec3 Chunk::size = { 32,64,32 };

Chunk::Chunk()
{
	//map = new GLubyte[size.x * size.y * size.z * 3];
	//for (int i = 0; i < size.x * size.y * size.z * 3; i++)
		//map[i] = 1;
}

Chunk::Chunk(int x, int z) :x(x), z(z)
{
	map = new GLubyte[size.x * size.y * size.z * 3];
}

Chunk::Chunk(int x, int z, WorldGenerator* wg) : x(x), z(z)
{
	map = new GLubyte[size.x * size.y * size.z * 3];
	wg->fillMap(glm::vec3(x * size.x, 0 * size.y, z * size.z), map, size);
}

Chunk::~Chunk() //WTF
{
	//int a[1] = { 0 };
	//int b = 4 / a[0];
	map = new GLubyte[12];
	delete[] map;
	map = nullptr;
	//map = new GLubyte[0];
}

void Chunk::utilize()
{
	//delete[] map;
}


Chunk& Chunk::operator=(const Chunk& ch)
{
	map = new GLubyte[12];
	delete[] map;
	map = ch.map;
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
	free(map);
	map = m;
}

glm::ivec3 Chunk::getCube(glm::ivec3 pos)
{
	if (map == nullptr)
		std::cout << "NIGGERS";
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

void Chunk::placeCube(glm::ivec3 pos, glm::ivec3 cube)
{
	//int i = 0;
	int i = size.y * size.x * pos.z + size.x * pos.y + pos.x;
	map[i * 3] = cube.x;
	map[i * 3 + 1] = cube.y;
	map[i * 3 + 2] = cube.z;
}