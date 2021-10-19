#include "World.h"

World::World() { map = nullptr; }

World::World(int w, int h, int d,  WorldGenerator* worldGen) : width(w), height(h), depth(d), worldGenerator(worldGen)
{
	map = new GLubyte[4 * width * height * depth];
	for (int i = 0; i < 4 * width * height * depth; i++)
		map[i] = 1;


	loadedChunks = new Chunk[load * load];
	loadedChunksChanged = new bool[load * load];
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++) 
		{
			loadedChunks[i * load + j] = getChunkAbs(i, 0, j);
			loadedChunksChanged[i * load + j] = false;
		}
}

World& World::operator=(const World& w)
{
	delete[] map;
	delete[] loadedChunks;
	delete[] loadedChunksChanged;

	width = w.width;
	height = w.height;
	depth = w.depth;
	map = w.map;
	loadedChunksChanged = w.loadedChunksChanged;
	loadedChunks = w.loadedChunks;
	worldGenerator = w.worldGenerator;
	return *this;
}


World::~World()
{
	map = new GLubyte[0];
	delete[] map;
	loadedChunks = new Chunk[0];
	delete[] loadedChunks;
	loadedChunksChanged = new bool[0];
	delete[] loadedChunksChanged;
}
void World::updateMapSampler()
{
	glm::ivec3 chunkSize = Chunk::getSize();
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++)
		{
			int x0 = chunkSize.x * i;
			int z0 = chunkSize.z * j;
			for (int x=x0; x<x0+chunkSize.x; x++)
				for (int y = 0; y < chunkSize.y; y++)
					for (int z = z0; z < z0 + chunkSize.z; z++)
					{
						int k = height * width * z + width * y + x;
						glm::ivec3 cube = loadedChunks[i * load + j].getCube(glm::ivec3(x-x0, y, z-z0));
						map[k * 4] = cube.x;
						map[k * 4 + 1] = cube.y;
					}
		}
	glGenTextures(1, &mapSampler);
	glBindTexture(GL_TEXTURE_3D, mapSampler);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV, map);
}

void World::updateMapSamplerS(glm::ivec3 pos)
{
	//std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;


	int k = height * width * pos.z + width * pos.y + pos.x;

	glm::ivec3 cube = getCube(pos);
	map[k * 4] = cube.x;
	map[k * 4 + 1] = cube.y;


	//glTexSubImage3D(GL_TEXTURE_3D, 0, pos.x, pos.y, pos.z, 1,1,1, GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV, map+k*4);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV, map);
}

GLuint World::getMapSampler()
{
	return mapSampler;
}

void World::update()
{
}

void World::saveChunks()
{
	for (int i = 0; i < load * load; i++)
		if(loadedChunksChanged[i])
			Database::placeChunk(loadedChunks[i]);
}

void World::placeCube(glm::ivec3 pos, glm::ivec3 cube)
{
	glm::ivec3 chunk = pos / Chunk::getSize();
	glm::ivec3 relPos = glm::ivec3(pos.x % Chunk::getSize().x, pos.y%Chunk::getSize().y, pos.z % Chunk::getSize().z);
	loadedChunks[chunk.x * load + chunk.z].placeCube(glm::ivec3(relPos.x, relPos.y, relPos.z), cube);
	loadedChunksChanged[chunk.x * load + chunk.z] = true;

	updateMapSamplerS(pos);
}

glm::ivec3 World::getCube(glm::ivec3 pos)
{
	if (pos.y >= Chunk::getSize().y || pos.y < 0)
		return glm::ivec3(999);
	glm::ivec3 chunk = pos / Chunk::getSize();
	glm::ivec3 relPos = glm::ivec3(pos.x % Chunk::getSize().x, pos.y, pos.z % Chunk::getSize().z);
	return loadedChunks[chunk.x * load + chunk.z].getCube(relPos);
}

GLubyte* World::getMap()
{
	return map;
}

glm::ivec3 World::getSize()
{
	return glm::ivec3(width, height, depth);
}

glm::ivec3 World::getChunkOffset()
{
	return chunkOffset;
}

Chunk World::getChunk(int x, int y, int z)
{
	int rx = x - chunkOffset.x;
	int rz = z - chunkOffset.z;
	if (rx >= 0 && rz >= 0 && rx < load && rz < load)
		return loadedChunks[rx * load + rz];
	else
		return getChunkAbs(x, y, z);
}

Chunk World::getChunkAbs(int x, int y, int z)
{
	Chunk* ch = Database::getChunk(x, y, z);
	if (ch == nullptr)
		return Chunk(x, z, worldGenerator);
	else
		return *ch;
}

void World::setWorldGenerator(WorldGenerator* gen)
{
	worldGenerator = gen;
}

glm::vec3 World::updateLoaded(glm::vec3 pos)
{

	glm::vec3 chunkSize = Chunk().getSize();
	glm::vec3 playerPos = pos;
	glm::vec2 deltaChunks(0);
	if (playerPos.x > chunkSize.x * (load / 2 + 1))
		playerPos.x -= chunkSize.x, deltaChunks.x++;
	if (playerPos.z > chunkSize.z * (load / 2 + 1))
		playerPos.z -= chunkSize.z, deltaChunks.y++;

	if (playerPos.x < chunkSize.x * (load - load / 2 - 1))
		playerPos.x += chunkSize.x, deltaChunks.x--;
	if (playerPos.z < chunkSize.z * (load - load / 2 - 1))
		playerPos.z += chunkSize.z, deltaChunks.y--;

	if (deltaChunks != glm::vec2(0))
	{
		saveChunks();
		Chunk* newLoadedChunks = new Chunk[load * load];
		for (int i = 0; i < load; i++)
			for (int j = 0; j < load; j++)
				newLoadedChunks[i * load + j] = getChunk(chunkOffset.x + i + deltaChunks.x, chunkOffset.y, chunkOffset.z + j + deltaChunks.y);
		delete[] loadedChunks;
		loadedChunks = newLoadedChunks;
		for (int i = 0; i < load * load; i++)
			loadedChunksChanged[i] = false;
		chunkOffset.x += deltaChunks.x;
		chunkOffset.z += deltaChunks.y;
		updateMapSampler();
	}
	return playerPos;

}

