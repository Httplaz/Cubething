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
#include "MeshRenderer.h"
#include <stack>
#include <deque>
#include <thread>

//class Chunk;

class World
{
	private:
		int* mipcdx;
		int* mipcdz;
		int* mipcdy;
		GLuint mapSampler;

		GLuint chunkOffsetSampler;
		int height;
		int width;
		int depth;
		GLubyte* map = nullptr;
		int load = 6;
		bool* loadNeeded;
		bool* generationNeeded;
		bool samplerLoaded = false;
		bool meshLoaded = false;
		glm::ivec3 chunkOffset = glm::vec3(0.,0.,0.);
		Chunk** loadedChunks;
		bool* loadedChunksChanged;
		bool* loadedChunksNeeded;
		WorldGenerator * worldGenerator = nullptr;
		int mipmapLevel = 5;
		int* chunkSamplerOffset;

		GLubyte** mipmap;
		
		std::deque <glm::ivec4> toSwap;

		MeshRenderer* meshRenderer;

		bool* toBuild;
		bool* toMap;

		bool* toUpdate;
		int chunkUpdatePeriod = 1;
		int maxChunkUpdatePeriod = 1;

		void streamSwapMapChunks(int i0, int j0, int i1, int j1);

	public:
		bool deb = false;
		World();
		World(int ld, WorldGenerator* worldGen, MeshRenderer* renderer);
		World& operator = (const World& w);
		World(const World& other);
		~World();
		GLuint getMapSampler();
		int* getChunkSamplerOffset();
		void updateMipmap();
		void updateMapSampler();
		void updateChunkSampler(int i, int j);
		void updateMapMesh();
		void debug();
		void reloadImage();
		void updateMapSamplerS(glm::ivec3 pos);
		void updateMapMeshS(glm::ivec3 pos);
		void update();
		void offthread();
		void startOffthread();
		void render();
		void saveChunks();
		void placeCube(glm::ivec3 pos, glm::ivec3 cube);
		void setMeshRenderer(MeshRenderer* renderer);
		MeshRenderer* getMeshRenderer();
		glm::ivec3 getCube(glm::ivec3 pos);
		GLubyte* getMap();
		glm::ivec3 getSize();
		glm::ivec3 getChunkOffset();
		Chunk* getChunk(int x, int y, int z);
		Chunk* getChunkAbs(int x, int y, int z);
		void setWorldGenerator(WorldGenerator* gen);
		bool processesFinished();
		glm::vec3 updateLoaded(glm::vec3 pos);
};


