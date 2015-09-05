// FirstPersonCamera.h
//
// @author Matthew Chan

#pragma once

#include "Camera.h"
#include "World.h"
#include "Grass.h"
#include "DoorModel.h"
class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(glm::vec3 position, World* world);
	virtual ~FirstPersonCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;
	const float cameraHeight = 2.5f;
private:
	glm::mat4 viewMatrix;
	glm::vec3 mLookAtPoint;
	glm::vec3 mUpVector;
	glm::vec3 mRightVector;
	glm::vec3 mPosition;
	World* world;

	float alpha;
	float beta;
	float radius;
	float strafeSpeed;
	float mouseSensitivity;
	float mouseMotionX;
	float mouseMotionY;

	float calcIntersectionDistance();
	bool isOutOfBounds();
	bool isWallCollision();
};
