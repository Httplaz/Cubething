#pragma once
#include <GL/glew.h>;
#include <glm/glm.hpp>
#include "WorldGenerator.h"
#include "Octree.h"
#define gf 2

class Chunk
{
	private:
		int x;
		int z;
		GLubyte* map = nullptr;
		static glm::ivec3 size;
		//static int width;
	public:
		Chunk();
		Chunk(int x, int z);
		Chunk(int x, int z, WorldGenerator* wg);
		~Chunk();
		void utilize();
		Chunk& operator = (const Chunk& ch);
		static glm::ivec3 getSize();
		GLubyte* getMap();
		void setMap(GLubyte* m);
		glm::ivec3 getCube(glm::ivec3 pos);
		glm::ivec3 getCube(int x, int y, int z);
		glm::ivec3 getPosition();
		void placeCube(glm::ivec3 pos, glm::ivec3 cube);
};

