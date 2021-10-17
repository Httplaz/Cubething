#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
private:
	glm::vec3 origin = glm::vec3(0., 0., 0.);
	glm::vec3 offset = glm::vec3(0., 0., 0.);
	float speed = 0.20f;
	glm::mat4 rotation = glm::mat4(1.0);
	glm::mat4 brotation = glm::mat4(1.0);
public:
	void setOrigin(glm::vec3 pos);
	glm::vec3 getOrigin();
	glm::mat4 getRotation();
	float getSpeed();
	void translate(glm::vec3 delta);
	void rotate(float deegrees, glm::vec4 dir);
	void rotateAbs(float deegrees, glm::vec4 dir);
};