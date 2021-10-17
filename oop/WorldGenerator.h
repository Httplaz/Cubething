#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <GLFW/glfw3.h>
class WorldGenerator
{
private:
	int height;
public:
	WorldGenerator();
	WorldGenerator(int h);
	glm::ivec2 getCube(glm::ivec3 pos);
	void fillMap(glm::vec3 pos, GLubyte* map, glm::ivec3 mapSize);
};

