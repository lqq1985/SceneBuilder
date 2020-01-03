#include "CameraFreeLook.h"
#include <iostream>

using namespace std;

CameraFreeLook::CameraFreeLook(int scrn_width, int scrn_height)
{
	this->screenWidthMid = scrn_width / 2.0f;
	this->screenHeightMid = scrn_height / 2.0f;

	this->updateVectors();
}

CameraFreeLook::~CameraFreeLook()
{
}

glm::mat4 CameraFreeLook::getViewMatrix()
{
	return glm::lookAt(this->position, this->position + this->frontVector, this->upVector);
}

void CameraFreeLook::mousePositionUpdate(float deltaTime, int x, int y)
{
	int xOffset = x - this->screenWidthMid;
	int yOffset = this->screenHeightMid - y;

	xOffset *= this->mouseSensitivity;
	yOffset *= this->mouseSensitivity;

	this->yaw += xOffset * deltaTime;
	this->pitch += yOffset * deltaTime;

	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;
	
	this->updateVectors();
}

void CameraFreeLook::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->frontVector = glm::normalize(front);
	this->rightVector = glm::normalize(glm::cross(this->frontVector, this->up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->upVector = glm::normalize(glm::cross(this->rightVector, this->frontVector));
}

void CameraFreeLook::forward(float deltaTime)
{
	this->position += this->frontVector * this->speed * deltaTime;
}

void CameraFreeLook::backward(float deltaTime)
{
	this->position -= this->frontVector * this->speed * deltaTime;
}

void CameraFreeLook::strafeRight(float deltaTime)
{
	this->position += glm::normalize(glm::cross(this->frontVector, this->up)) * this->speed * deltaTime;
}

void CameraFreeLook::strafeLeft(float deltaTime)
{
	this->position -= glm::normalize(glm::cross(this->frontVector, this->up)) * this->speed * deltaTime;
}

glm::vec3 CameraFreeLook::getCameraPosition()
{
	return this->position;
}

void CameraFreeLook::updatePosition(glm::vec3 position)
{
	this->position = position;
}
