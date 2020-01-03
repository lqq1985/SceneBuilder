/*
CameraFP

A camera/view class that acts like an FPS camera.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraFreeLook
{
public:
	CameraFreeLook(int scrn_width, int scrn_height);
	~CameraFreeLook();
	glm::mat4 getViewMatrix();
	void forward(float deltaTime);
	void backward(float deltaTime);
	void strafeRight(float deltaTime);
	void strafeLeft(float deltaTime);
	void mousePositionUpdate(float deltaTime, int x, int y);
	glm::vec3 getCameraPosition();
	void updatePosition(glm::vec3 position);
	float yaw = -89.0f; // this is typicall where the view faces towards. -90 degrees is facing towards the z axis
	float pitch = -45.0f;
private:
	float speed = 2.0f;
	float mouseSensitivity = 2.0f;
	int screenWidthMid;
	int screenHeightMid;

	glm::vec3 position = glm::vec3(0.0f, 22.0f, 32.0f); // position of camera in the world
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 frontVector;
	glm::vec3 rightVector;
	glm::vec3 upVector;

	void updateVectors();
};

#endif