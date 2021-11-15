#pragma once
#define GLFW_INCLUDE_NONE

// GLFW
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/detail/compute_vector_relational.hpp>
#include "Shader.h"
#include "Texture.h"
//#include <glm/detail/func_vector_relational.inl>

class MeshRenderer
{
private:
	GLuint VBO, VAO, EBO, FBO, quadVAO, quadVBO, TVAO, TVBO;
	std::map<int, glm::ivec2> meshRegions;
	Shader screenShader;
	Shader tshader;
	//Texture tex;
	GLubyte* test;
	GLuint tex;
public:
	int verticesToDraw = 0;
	std::vector<GLfloat> vertices;
	int verticesSize = 0;
	MeshRenderer();
	~MeshRenderer();
	void createScreenMesh();
	void prepareFrameBuffer();
	void closeFrameBuffer();
	void postRender();
	void createWorldMesh(GLubyte* map, int width, int height, int depth);
	void clearMesh();
	void addWorldMesh(GLubyte* map, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth);
	void replaceWorldMesh(GLubyte* map, glm::ivec3 p0, glm::ivec3 p1, int width, int height, int depth);
	void createMesh();
	void render();
};

