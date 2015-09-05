// FirstPersonCamera.cpp
//
// @author Matthew Chan

#include "FirstPersonCamera.h"
#include "EventManager.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "glm/ext.hpp"
#include <iostream>
#include "Grass.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>


using namespace glm;
using namespace std;

FirstPersonCamera::FirstPersonCamera(glm::vec3 position, World* world)
: Camera(), mPosition(position), mLookAtPoint(0, 0, 0), mUpVector(0, 1, 0), alpha(0), beta(0), radius(1), strafeSpeed(2), mouseSensitivity(25)
{
	this->world = world;
}
FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Update(float dt)
{
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();

	mouseMotionX = -EventManager::GetMouseMotionX() * mouseSensitivity * dt;
	mouseMotionY = EventManager::GetMouseMotionY() * mouseSensitivity * dt;


	beta = fmod(beta + mouseMotionX, 360);
	alpha = glm::clamp(
		(alpha + mouseMotionY),
		-85.0f,
		85.0f
		);

	mLookAtPoint.x = radius * glm::cos(glm::radians(alpha)) * glm::sin(glm::radians(beta));
	mLookAtPoint.y = radius * glm::sin(glm::radians(alpha));
	mLookAtPoint.z = radius * glm::cos(glm::radians(alpha)) * glm::cos(glm::radians(beta));

	mRightVector = glm::vec3(
		glm::sin(glm::radians(beta - 90)),
		0,
		glm::cos(glm::radians(beta - 90))
		);

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		strafeSpeed = 6;
	}
	else
	{
		strafeSpeed = 2;
	}

	float pDist = 0.f;
	float adjustY = 0.f;

	pDist = calcIntersectionDistance();
	if (cameraHeight != pDist)
	{
		if (pDist > cameraHeight){
			adjustY = pDist - cameraHeight;
		}
		else
		{
			adjustY = cameraHeight - pDist;
		}
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		mPosition += mRightVector * strafeSpeed * dt;
		if (isOutOfBounds())
		{
			mPosition -= mRightVector * strafeSpeed * dt;
		}
		else if (isWallCollision())
		{
			mPosition -= mRightVector * strafeSpeed * dt;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		mPosition += mLookAtPoint * strafeSpeed * dt;
		if (mPosition.y > adjustY)
		{
			float oldY = mPosition.y;
			mPosition.y = mPosition.y - (oldY - adjustY);
		}
		else
		{
			float oldY = mPosition.y;
			mPosition.y = mPosition.y + (adjustY - oldY);
		}
		if (isOutOfBounds())
		{
			mPosition -= mLookAtPoint * strafeSpeed * dt;
		}
		else if (isWallCollision())
		{
			mPosition -= mLookAtPoint * strafeSpeed * dt;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		mPosition -= mRightVector * strafeSpeed * dt;
		if (isOutOfBounds())
		{
			mPosition += mRightVector * strafeSpeed * dt;
		}
		else if (isWallCollision())
		{
			mPosition += mRightVector * strafeSpeed * dt;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		mPosition -= mLookAtPoint * strafeSpeed * dt;
		if (mPosition.y > adjustY)
		{
			float oldY = mPosition.y;
			mPosition.y = mPosition.y - (oldY - adjustY);
		}
		else
		{
			float oldY = mPosition.y;
			mPosition.y = mPosition.y + (adjustY - oldY);
		}

		if (isOutOfBounds())
		{
			mPosition += mLookAtPoint * strafeSpeed * dt;
		}
		else if (isWallCollision())
		{
			mPosition += mLookAtPoint * strafeSpeed * dt;
		}
	}
}

glm::mat4 FirstPersonCamera::GetViewMatrix() const
{

	return glm::lookAt(mPosition,		// Camera position
		mPosition - mLookAtPoint,	// Look towards this point
		mUpVector		// Up vector
		);
}

float FirstPersonCamera::calcIntersectionDistance()
{
	float pDist;
	glm::vec3 cameraPos = glm::vec3(glm::inverse(this->GetViewMatrix())[3]);
	glm::vec3 groundPos = this->world->GetGrassModel()->GetPosition();
	glm::vec3 ray;
	float y = cameraPos.y;
	if (y < 0)
	{
		ray = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z);
	}
	else
	{
		ray = glm::vec3(cameraPos.x, -cameraPos.y, cameraPos.z);
		ray = glm::normalize(ray);
	}
	//normal of floor
	glm::vec3 vecNormal = glm::vec3(0.0f, 1.0f, 0.0f);

	//Calculate a vector from the point on the plane to our test point
	vec3 vecTemp = ((cameraPos - groundPos));

	//Calculate the distance: dot product of the new vector with the plane's normal
	pDist = glm::normalize((glm::dot(vecTemp, vecNormal)));

	//for reference....
	/*
		if (pDist > 0.0f)
		Point is in front of the plane

		if (pDist < -0.0f)
		Point is behind the plane

		//If neither of these were true, then the point is on the plane
		*/

	return pDist;
}

bool FirstPersonCamera::isOutOfBounds()
{
	glm::vec3 pointOnWall = vec3(0.0f, 0.0f, -24.0f);
	glm::vec3 normalOnPlane = vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 ceneterOfSphere = glm::vec3(glm::inverse(this->GetViewMatrix())[3]);
	glm::vec3 dVecFromPlaneToSphere = ceneterOfSphere - pointOnWall;

	//Calculate the distance: dot product of the new vector with the plane's normal
	float fDist = (glm::dot(dVecFromPlaneToSphere, normalOnPlane));

	if (fDist > 0){
		//no intersection
	}
	else
	{
		printf("outOfBounds");
		return true;
	}
	pointOnWall = vec3(0.0f, 0.0f, 24.0f);
	normalOnPlane = vec3(0.0f, 0.0f, -1.0f);
	ceneterOfSphere = glm::vec3(glm::inverse(this->GetViewMatrix())[3]);
	dVecFromPlaneToSphere = ceneterOfSphere - pointOnWall;

	//Calculate the distance: dot product of the new vector with the plane's normal
	fDist = (glm::dot(dVecFromPlaneToSphere, normalOnPlane));

	if (fDist > 0){
		//no intersection
	}
	else
	{
		printf("outOfBounds");
		return true;
	}

	pointOnWall = vec3(24.0f, 0.0f, 0.0f);
	normalOnPlane = vec3(-1.0f, 0.0f, 0.0f);
	ceneterOfSphere = glm::vec3(glm::inverse(this->GetViewMatrix())[3]);
	dVecFromPlaneToSphere = ceneterOfSphere - pointOnWall;

	//Calculate the distance: dot product of the new vector with the plane's normal
	fDist = (glm::dot(dVecFromPlaneToSphere, normalOnPlane));

	if (fDist > 0){
		//no intersection
	}
	else
	{
		printf("outOfBounds");
		return true;
	}

	pointOnWall = vec3(-24.0f, 0.0f, 0.0f);
	normalOnPlane = vec3(1.0f, 0.0f, 0.0f);
	ceneterOfSphere = glm::vec3(glm::inverse(this->GetViewMatrix())[3]);
	dVecFromPlaneToSphere = ceneterOfSphere - pointOnWall;

	//Calculate the distance: dot product of the new vector with the plane's normal
	fDist = (glm::dot(dVecFromPlaneToSphere, normalOnPlane));

	if (fDist > 0){
		//no intersection
	}
	else
	{
		printf("outOfBounds");
		return true;
	}

	return false;
}

bool FirstPersonCamera::isWallCollision()
{
	glm::vec3 pointOnWall = vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 normalOnPlane = vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 centerOfSphere = glm::vec3(glm::inverse(this->GetViewMatrix())[3]);
	glm::vec3 dVecFromPlaneToSphere = centerOfSphere - pointOnWall;

	//Calculate the distance: dot product of the new vector with the plane's normal
	float fDist = (glm::dot(dVecFromPlaneToSphere, normalOnPlane));

	if (fDist > 0){
		//no intersection
		return false;
	}
	else
	{
		//determine if current position is on the actual wall plane
		if (centerOfSphere.x > -5.0f && centerOfSphere.x < 4.4f){
			//front wall
			if (centerOfSphere.z < 4.8f && centerOfSphere.z > 4.0f)
			{
				return true;
			}
			//back wall
			if (centerOfSphere.z < -4.0f && centerOfSphere.z > -5.4f){
				return true;
			}
		}
		if (centerOfSphere.z > -5.4f && centerOfSphere.z <4.8f)
		{
			//left wall
			if (centerOfSphere.x < -3.2f && centerOfSphere.x > -4.3f){
				return true;
			}
		}

		if (this->world->GetDoorModel()->isDoorOpen)
		{

			if (centerOfSphere.z > -5.2f && centerOfSphere.z <-3.0f)
			{
				//right wall 1
				if (centerOfSphere.x < 4.2f && centerOfSphere.x > 3.2f){
					return true;
				}
			}
			if (centerOfSphere.z > -1.4f && centerOfSphere.z < 4.8f)
			{
				//right wall 2
				if (centerOfSphere.x < 4.2f && centerOfSphere.x > 3.2f){
					return true;
				}
			}
		}
		else
		{
			if (centerOfSphere.z > -5.4f && centerOfSphere.z <4.8f)
			{
				//all of right wall
				if (centerOfSphere.x < 4.2f && centerOfSphere.x > 3.2f){
					return true;
				}
			}
		}

		if (centerOfSphere.z > -3.5f && centerOfSphere.z <-0.5f)
		{
			//inside z wall
			if (centerOfSphere.x < 0.95f && centerOfSphere.x > -0.5f){
				return true;
			}
		}
		if (centerOfSphere.x > -2.5f && centerOfSphere.x <1.8f)
		{
			//inside x wall
			if (centerOfSphere.z < 0.8f && centerOfSphere.z > 0.2f){
				return true;
	}
}
		
		return false;
	}
}
