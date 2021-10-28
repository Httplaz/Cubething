#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace glm;
class Physics
{
private:
public:
	static glm::ivec3 raycastCube(glm::vec3 pos, glm::vec3 dir, GLubyte* map, glm::ivec3 size);
	static glm::vec3 raycastCubeRaw(glm::vec3 pos, glm::vec3 dir, GLubyte* map, glm::ivec3 size);
	static glm::vec3 collideEntity(glm::vec3 pos, glm::vec3 delta, glm::mat4 rotation, GLubyte* map, glm::ivec3 size);
	static glm::ivec3 getCube(glm::ivec3 pos, GLubyte* map, glm::ivec3 mapSize);
	static glm::ivec3 getCubeNormale(glm::vec3 pos, glm::vec3 cube);
	static glm::ivec3 cubeNormale(int n);
	static bool checkIntersection(glm::ivec3 vpos, glm::vec3 pos);
};

