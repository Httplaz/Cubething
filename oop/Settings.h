#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"
#include "World.h"
#include "WorldGenerator.h"
#include "Camera.h"
#include "Physics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Player.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
class Settings
{
private:
	glm::vec3 playerPosition = glm::vec3(110., 34., 110.);
	glm::mat4 playerRotation = glm::mat4(1.);
	glm::mat4 playerCameraRotation = glm::mat4(1.);
	float renderDistanceSteps = 40;
	float renderDistanceMeters = 150;
	int loadingDistance = 3;
	float playerSpeed = 40.f;
public:
	glm::vec3 getPlayerPosition();
	glm::mat4 getPlayerRotation();
	glm::mat4 getPlayerCameraRotation();
	float getRenderDistanceSteps();
	float getRenderDistanceMeters();
	int getLoadingDistance();
	float getPlayerSpeed();

	void saveSettings();
	void loadSettings();

	void setPlayerPosition(glm::vec3 p);
	void setPlayerRotation(glm::mat4 m);
	void setPlayerCameraRotation(glm::mat4 m);
	void setRenderDistanceSteps(int d);
	void setRenderDistanceMeters(int d);
	void setLoadingDistance(int d);
	void setPlayerSpeed(float s);
};

