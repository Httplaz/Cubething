// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Chunk.h"

glm::ivec3 Chunk::size = { 32,64,32 };

Chunk::Chunk(const Chunk& other)
{
	if (map != nullptr)
		delete[] map, map = nullptr;
	if (other.map != nullptr)
	{
		map = new GLubyte[size.x * size.y * size.z * 3];
		for (int i = 0; i < size.x * size.y * size.z * 3; i++)
			map[i] = other.map[i];
	}
	x = other.x;
	z = other.z;
}

Chunk::Chunk()
{
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
}

Chunk::~Chunk()
{
	if (map != nullptr) 
	{
		delete[] map;
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
	free(map);
	map = m;
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

void Chunk::placeCube(glm::ivec3 pos, glm::ivec3 cube)
{
	int i = size.y * size.x * pos.z + size.x * pos.y + pos.x;
	map[i * 3] = cube.x;
	map[i * 3 + 1] = cube.y;
	map[i * 3 + 2] = cube.z;
}