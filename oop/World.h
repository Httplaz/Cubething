#pragma once
#include <GL/glew.h>;
#include <glm/glm.hpp>
#include "Chunk.h"
#include "WorldGenerator.h"
#include "Database.h"
#include <iostream>;

//class Chunk;

class World
{
	private:
		GLuint mapSampler;
		int height;
		int width;
		int depth;
		GLubyte* map = nullptr;
		int load = 6;
		bool samplerLoaded = false;
		glm::ivec3 chunkOffset = glm::vec3(0.,0.,0.);
		Chunk* loadedChunks;
		bool* loadedChunksChanged;
		WorldGenerator * worldGenerator = nullptr;
		int mipmapLevel = 5;

		GLubyte** mipmap;

	public:
		World();
		World(int w, int h, int d, WorldGenerator* worldGen);
		World& operator = (const World& w);
		~World();
		GLuint getMapSampler();
		void updateMapSampler();
		void updateMapSamplerS(glm::ivec3 pos);
		void update();
		void saveChunks();
		void placeCube(glm::ivec3 pos, glm::ivec3 cube);
		glm::ivec3 getCube(glm::ivec3 pos);
		GLubyte* getMap();
		glm::ivec3 getSize();
		glm::ivec3 getChunkOffset();
		Chunk getChunk(int x, int y, int z);
		Chunk getChunkAbs(int x, int y, int z);
		void setWorldGenerator(WorldGenerator* gen);
		glm::vec3 updateLoaded(glm::vec3 pos);
};


