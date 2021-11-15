// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Player.h"

Player::Player(World* w, Camera* c, glm::vec3 p) : world(w), position(p), camera(c)
{
	rotation = glm::mat4(0.);
	velocity = glm::vec3(0);
	movement = glm::vec3(0);
	breakDelay = 10;
	placeDelay = 10;
	shift = false;
	sprint = false;
	grounded = false;
	flight = true;
	speed = 0.2f;

	selectedCube = glm::ivec3(0);
	selectedCubeNormale = glm::ivec3(0);
}

void Player::setShift(bool a)
{
	shift = a;
}

void Player::setSprint(bool a)
{
	sprint = a;
}

void Player::setFlight(bool a)
{
	flight = a;
}

void Player::setSpeed(float s)
{
	speed = s;
}

void Player::setPosition(glm::vec3 p)
{
	position = p;
}

void Player::setRotation(glm::mat4 m)
{
	rotation = m;
}

void Player::update()
{
	placeDelay -= (placeDelay > 0);
	breakDelay -= (breakDelay > 0);



	

	if (Physics::collideEntity(position, glm::vec3(0, -0.3, 0), camera->getRotation(), world->getMap(), world->getSize()) == position && !flight)
		grounded = true;
	else
		grounded = false;


	if (!flight)
		velocity += glm::vec3(0, -0.02, 0);
	if (grounded)
		velocity.y = max(0.f, velocity.y);

	
	if (Physics::collideEntity(position, glm::vec3(0, 0.4, 0), camera->getRotation(), world->getMap(), world->getSize()) == position && !flight)
		velocity.y = min(velocity.y, -0.1f);


	glm::vec3 posBackup = position;
	//position += (movement * (shift ? 0.5f : 1.f) + velocity)* speed;
	position = Physics::collideEntity(position, (movement * (shift ? 0.5f : 1.f) + velocity) * speed, camera->getRotation(), world->getMap(), world->getSize());
	//if ((grounded && shift && !(Physics::collideEntity(position, glm::vec3(0, -0.4, 0), camera->getRotation(), world->getMap(), world->getSize()) == position)))
		//position = posBackup;
	position = world->updateLoaded(position);
	camera->setOrigin(position);
	

	glm::vec4 dir = glm::vec4(0., 0., 1., 0.) * camera->getRotation();
	selectedCube = Physics::raycastCube(camera->getOrigin(), glm::vec3(dir.x, dir.y, dir.z), world->getMap(), world->getSize());
	glm::vec3 selectorRayPos = Physics::raycastCubeRaw(camera->getOrigin(), glm::vec3(dir.x, dir.y, dir.z), world->getMap(), world->getSize());
	glm::ivec3 norm = Physics::getCubeNormale(selectorRayPos, selectedCube);
	selectedCubeNormale = Physics::cubeNormale(norm.z);
}

void Player::attack()
{
	if (breakDelay == 0 && (grounded || flight))
	{
		glm::ivec3 cube = selectedCube;
		if (cube.x != 999)
			world->placeCube(cube, glm::ivec3(0, 0, 0)), breakDelay = 0;
	}
}

void Player::place()
{
	if (placeDelay == 0)
	{
		glm::ivec3 cube = selectedCube;
		if (cube.x != 999)
			world->placeCube(cube + selectedCubeNormale, placeable), placeDelay = 9;
	}
}

void Player::jump()
{
	if (grounded)
		velocity.y = 0.7;
}

void Player::setVelocity(glm::vec3 v)
{
	velocity = v;
}

void Player::addVelocity(glm::vec3 dv)
{
	velocity+=dv;
}

void Player::selectPlaceable(glm::ivec3 pl)
{
	placeable = pl;
}

bool Player::isFlying()
{
	return flight;
}

bool Player::isShifting()
{
	return shift;
}

glm::mat4 Player::getRotation()
{
	return rotation;
}

void Player::setMovement(glm::vec3 v)
{
	glm::vec4 deltaTemp = glm::vec4(v.x, v.y, v.z, 0.) * camera->getRotation();//(flight ? camera->getRotation() : rotation);
	v = glm::vec3(deltaTemp.x, deltaTemp.y*flight, deltaTemp.z);
	movement = v;
}

void Player::addMovement(glm::vec3 dv)
{
	glm::vec4 deltaTemp = glm::vec4(dv.x, dv.y, dv.z, 0.) * (flight? camera->getRotation() : rotation);
	dv = glm::vec3(deltaTemp.x, deltaTemp.y*flight, deltaTemp.z);
	movement += dv;
}

glm::vec3 Player::getPosition()
{
	return position;
}

void Player::rotate(float deegrees, glm::vec4 dir)
{
	camera->rotate(deegrees, dir);
	dir = glm::vec4(dir.x, 0, dir.z, dir.w) * rotation + glm::vec4(0, dir.y, 0, 0);
	//rotation = glm::rotate(rotation, glm::radians(deegrees), glm::vec3(dir.x, dir.y, dir.z));
}
