#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "WorldGenerator.h"
#include <iostream>
#include <vector>
#include <thread>
#define gf 2

class Chunk
{
	private:
		int x;
		int z;
		GLubyte* map = nullptr;
		static glm::ivec3 size;
		GLuint VAO, VBO;
		int loadingStatus = 0;
		std::vector<GLubyte> v;
		bool GLGenerated = false;
	public:
		std::vector<GLfloat> vertices;
		GLuint verticesSize = 0;
		GLuint verticesToDraw = 0;
		Chunk(const Chunk& other);
		Chunk();
		Chunk(int x, int z);
		Chunk(int x, int z, WorldGenerator* wg);
		~Chunk();
		Chunk& operator = (const Chunk& ch);
		static glm::ivec3 getSize();
		GLubyte* getMap();
		void setMap(GLubyte* m);
		void setVertices(std::vector<GLfloat> vertices);
		std::vector<GLfloat> buildMesh(GLubyte* m, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth, Chunk* chunk);
		void prepareRender(GLubyte* m, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth);
		void loadMesh();
		void moveMesh(glm::ivec2 delta);
		void clearMesh();
		void render();
		void endRender();
		void release();
		glm::ivec3 getCube(glm::ivec3 pos);
		glm::ivec3 getCube(int x, int y, int z);
		glm::ivec3 getPosition();
		int getLoadingStatus();
		void stLoadingStatus(int ls);
		void placeCube(glm::ivec3 pos, glm::ivec3 cube);
		int test;
};

