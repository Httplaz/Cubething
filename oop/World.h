#pragma once
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Chunk.h"
#include "WorldGenerator.h"
#include "Database.h"
#include <iostream>;
#include <vector>
#include <algorithm>

//class Chunk;

class World
{
	private:
		int* mipcdx;
		int* mipcdz;
		int* mipcdy;
		GLuint mapSampler;
		int height;
		int width;
		int depth;
		GLubyte* map = nullptr;
		int load = 6;
		bool* loadNeeded;
		bool* generationNeeded;
		bool samplerLoaded = false;
		glm::ivec3 chunkOffset = glm::vec3(0.,0.,0.);
		Chunk** loadedChunks;
		bool* loadedChunksChanged;
		WorldGenerator * worldGenerator = nullptr;
		int mipmapLevel = 5;

		GLubyte** mipmap;

		void streamSwapMapChunks(int i0, int j0, int i1, int j1);

	public:
		World();
		World(int ld, WorldGenerator* worldGen);
		World(int w, int h, int d, WorldGenerator* worldGen);
		World& operator = (const World& w);
		World(const World& other);
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
		Chunk* getChunk(int x, int y, int z);
		Chunk* getChunkAbs(int x, int y, int z);
		void setWorldGenerator(WorldGenerator* gen);
		glm::vec3 updateLoaded(glm::vec3 pos);
};


