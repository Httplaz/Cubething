#pragma once
#include <GL/glew.h>;
#include <glm/glm.hpp>
#include <iostream>;
#include <stack>;
class Octree
{
private:
	GLubyte* map = nullptr;
	GLubyte* extents = nullptr;
	GLubyte maxSide;
	GLubyte extent;
public:
	Octree(int extent);
	~Octree();
	void getPtr(glm::ivec3 pos);
	void placeCube(glm::ivec3 pos, bool cube);
};

