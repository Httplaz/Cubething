#pragma once
#include "Camera.h"
#include "World.h"
#include "Physics.h"
class Player
{
	private:
		Camera* camera;
		World* world;
		glm::mat4 rotation;
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 movement;
		glm::ivec3 selectedCube;
		glm::ivec3 selectedCubeNormale;
		bool grounded;
		bool shift;
		bool sprint;
		bool flight;
		float speed;
		int placeDelay;
		int breakDelay;
		glm::ivec3 placeable;
public:
	Player(World* w, Camera* c, glm::vec3 p);
	void setShift(bool a);
	void setSprint(bool a);
	void setFlight(bool a);
	void setPosition(glm::vec3 p);
	void update();
	void jump();
	void attack();
	void place();
	void rotate(float deegrees, glm::vec4 dir);
	void setVelocity(glm::vec3 v);
	void addVelocity(glm::vec3 dv);
	void setMovement(glm::vec3 v);
	void addMovement(glm::vec3 dv);
	void selectPlaceable(glm::ivec3 pl);
	glm::vec3 getPosition();
	bool isFlying();
	bool isShifting();
};

