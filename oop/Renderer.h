#pragma once
#include "Shader.h"
#include "MeshRenderer.h"
#include "Texture.h"
class Renderer
{
private:
	MeshRenderer* meshRenderer;
	Shader screenShader;
	Shader gameShader;
	Texture textureAtlas;
	Texture normaleAtlas;
	Texture skySphere;
	GLuint FBO, RBO, bufferTexture, width=1920, height=1080;
public:
	Renderer(Shader ssh, Shader gs, Texture ta, Texture na, Texture ssp, int w, int h);
	void start();
	void render();
	void renderGui();
	void renderGame();
	void postRenderGame();
	Shader* getGameShader();
	Shader* getScreenShader();
	MeshRenderer* getMeshRenderer();
	void setScreenSize(glm::ivec2 s);
};

