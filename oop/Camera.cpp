// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Camera.h"

void Camera::setOrigin(glm::vec3 pos)
{
	origin = pos;
}

glm::vec3 Camera::getOrigin()
{
	return origin;
	glm::mat4 d1(0);
	glm::mat4 d2(0);
	glm::mat4 d3 = d1 + d2;
}

glm::mat4 Camera::getRotation()
{
	return rotation;
}

glm::mat4 Camera::getRotationP()
{
	return rotationP;
}

void Camera::setRotation(glm::mat4 m)
{
	rotation = m;
}

float Camera::getSpeed()
{
	return speed;
}

void Camera::translate(glm::vec3 delta)
{
	glm::vec4 temp = glm::vec4(delta.x, delta.y, delta.z, 0.) * rotation * speed;
	origin += glm::vec3(temp.x, temp.y, temp.z);
}

void Camera::rotate(float deegrees, glm::vec4 dir)
{
	dir = glm::vec4(dir.x,0,dir.z,dir.w) * rotation + glm::vec4(0,dir.y,0,0);
	rotation = glm::rotate(rotation, glm::radians(deegrees), glm::vec3(dir.x, dir.y, dir.z));
	rotationP = glm::rotate(rotationP, glm::radians(-deegrees), glm::vec3(dir.x, dir.y, dir.z));
}

void Camera::rotateAbs(float deegrees, glm::vec4 dir)
{
	rotation = glm::rotate(rotation, glm::radians(deegrees), glm::vec3(dir.x, dir.y, dir.z));
}
