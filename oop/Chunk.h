#pragma once
#include <GL/glew.h>;
#include <glm/glm.hpp>
#include "WorldGenerator.h"

class Chunk
{
	private:
		int x;
		int z;
		GLubyte* map;
		static glm::ivec3 size;
		//static int width;
	public:
		Chunk();
		Chunk(int x, int z);
		Chunk(int x, int z, WorldGenerator* wg);
		~Chunk();
		Chunk& operator = (const Chunk& ch);
		static glm::ivec3 getSize();
		GLubyte* getMap();
		void setMap(GLubyte* m);
		glm::ivec3 getCube(glm::ivec3 pos);
		glm::ivec3 getCube(int x, int y, int z);
		glm::ivec3 getPosition();
		void placeCube(glm::ivec3 pos, glm::ivec3 cube);
};
