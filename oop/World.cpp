#include "World.h"

World::World() { map = nullptr; }

World::World(int w, int h, int d,  WorldGenerator* worldGen) : width(w), height(h), depth(d), worldGenerator(worldGen)
{
	map = new GLubyte[3 * width * height * depth];
	for (int i = 0; i < 3 * width * height * depth; i++)
		map[i] = 1;

	//worldGenerator->fillMap(glm::vec3(0.), map, this->getSize());

	loadedChunks = new Chunk[load * load];
	//glm::ivec3 cube = loadedChunks[8].getCube(15, 15, 15);
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++) 
		{
			loadedChunks[i * load + j] = Chunk(i,j, worldGenerator);
			loadedChunks[i * load + j] = getChunk(i, 0, j);
			//worldGenerator->fillMap(glm::vec3(0.), loadedChunks[i*load+j].getMap(), loadedChunks[i * load + j].getSize());
		}
}

World& World::operator=(const World& w)
{
	delete[] map;
	delete[] loadedChunks;

	width = w.width;
	height = w.height;
	depth = w.depth;
	map = w.map;
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
	//if(map!=nullptr)
	//delete[] map;
	map = nullptr;
}
void World::updateMapSampler()
{
	glm::ivec3 chunkSize = Chunk().getSize();
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++)
		{
			int x0 = chunkSize.x * i;
			int z0 = chunkSize.z * j;
			for (int x=x0; x<x0+chunkSize.x; x++)
				for (int y = 0; y < chunkSize.y; y++)
					for (int z = z0; z < z0 + chunkSize.z; z++)
					{
						//nt k = height * width * z + width * y + x;
						int k = height * width * z + width * y + x;
						glm::ivec3 cube = loadedChunks[i * load + j].getCube(glm::ivec3(x-x0, y, z-z0));
						map[k * 3] = cube.x;
						map[k * 3 + 1] = cube.y;
					}
		}
	glGenTextures(1, &mapSampler);
	glBindTexture(GL_TEXTURE_3D, mapSampler);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width, height, depth, 0, GL_RGB, GL_UNSIGNED_BYTE, map);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, map);
	glGenerateMipmap(GL_TEXTURE_2D);
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
		Database::placeChunk(loadedChunks[i]);
}

void World::placeCube(glm::ivec3 pos, glm::ivec3 cube)
{
	glm::ivec3 chunk = pos / Chunk::getSize();
	glm::ivec3 relPos = glm::ivec3(pos.x % Chunk::getSize().x, pos.y%Chunk::getSize().y, pos.z % Chunk::getSize().z);
	loadedChunks[chunk.x * load + chunk.z].placeCube(glm::ivec3(relPos.x, relPos.y, relPos.z), cube);
	//Database::placeChunk(loadedChunks[chunk.x * load + chunk.z]);
	//glm::ivec3 p = loadedChunks[chunk.x * load + chunk.z].getPosition();
	//loadedChunks[chunk.x * load + chunk.z] = *Database::getChunk(p.x, p.y, p.z);


	updateMapSampler();
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
	//else
		//return Chunk(x,z,worldGenerator);
	Chunk* ch = Database::getChunk(x,y,z);
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
	//glm::vec3 camPos = camera->getOrigin();
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

	//camera->setOrigin(camPos);
	if (deltaChunks != glm::vec2(0))
	{
		saveChunks();
		Chunk* newLoadedChunks = new Chunk[load * load];
		for (int i = 0; i < load; i++)
			for (int j = 0; j < load; j++)
				newLoadedChunks[i * load + j] = getChunk(chunkOffset.x + i + deltaChunks.x, chunkOffset.y, chunkOffset.z + j + deltaChunks.y);
		delete[] loadedChunks;
		loadedChunks = newLoadedChunks;
		chunkOffset.x += deltaChunks.x;
		chunkOffset.z += deltaChunks.y;
		updateMapSampler();
	}
	return playerPos;

}

