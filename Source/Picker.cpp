// Picker.cpp
// Purpose: This class  keeps a collection of pickable objects and decides
// when an object has been picked given certain screen coordinates.
//
// @author Michael Lavoie

#include "Picker.h"

#include <GLFW/glfw3.h>
#include "glm/ext.hpp"
#include <iostream>

#include "EventManager.h"
#include "Camera.h"
#include "BoundingBox.h"
#include "Model.h"



Picker::~Picker()
{
}

void Picker::registerPickable(IPickable* pickable)
{
	pickables.push_back(pickable);
}

void Picker::pick(double xPos, double yPos, Camera* camera)
{
	// Get the current window dimensions
	int W, H;
	glfwGetWindowSize(EventManager::GetWindow(), &W, &H);

	// Calculate the NDC coordinates
	float xN = (2 / (float)W) * xPos + ((1 - (float)W) / (float)W);
	float yN = -(2 / (float)H) * yPos - ((1 - (float)H) / (float)H);
	
	glm::vec4 rayClip = glm::vec4(xN, yN, -0.1f, 1.0f);

	glm::vec4 rayView = glm::inverse(camera->GetProjectionMatrix()) * rayClip;
	rayView = glm::vec4(rayView.x, rayView.y, -1.0f, 0.0f);

	glm::vec3 rayWorld = glm::vec3(glm::inverse(camera->GetViewMatrix()) * rayView);
	rayWorld = glm::normalize(rayWorld);
	
	glm::vec3 C = glm::vec3(glm::inverse(camera->GetViewMatrix())[3]);
	
	glm::vec4 tempIntersection = glm::vec4(0, 0, 0, 0);
	glm::vec4 intersection = glm::vec4(0, 0, 0, 0);
	IPickable* intersectedObject = nullptr;

	for (vector<IPickable*>::iterator it = pickables.begin(); it != pickables.end(); ++it)
	{
		tempIntersection = intersectionPoint((*it), glm::vec4(rayWorld, 0), glm::vec4(C, 1));

		if (tempIntersection.w >= 0 && (tempIntersection.w < intersection.w || intersection.w == 0)) // There was an intersection
		{
			intersection = tempIntersection;
			intersectedObject = (*it);
		}
	}

	if (intersectedObject != nullptr)
	{
		intersectedObject->Picked();
	}
	else
	{
		cout << endl << "Nothing picked!" << endl;
	}
}

glm::vec4 Picker::intersectionPoint(IPickable* pickable, glm::vec4 rayVector, glm::vec4 p0)
{
	glm::vec2 xRange, yRange, zRange;
	float ERROR = 0.0001;


	cout << endl << glm::to_string(dynamic_cast<Model*>(pickable)->GetWorldMatrix()) << endl;

	vector<glm::vec4> planes = BoundingBox::CreateAABoundingBox(pickable->GetWorldVertices(), xRange, yRange, zRange, dynamic_cast<Model*>(pickable)->GetWorldMatrix());

	if (planes.empty())
		return {};

	// The distance from p0 to the intersection
	float shortestT = -1;
	float tempT = -glm::dot(planes[0], p0) / glm::dot(planes[0], rayVector);

	// Set w to -1 because we haven't found a point yet
	glm::vec4 closestIntersect = glm::vec4(0, 0, 0, shortestT);
	glm::vec4 tempIntersect = p0 + (tempT * rayVector);

	if (tempT >= 0 &&
		tempIntersect.x >= xRange.x - ERROR && tempIntersect.x <= xRange.y + ERROR &&
		tempIntersect.y >= yRange.x - ERROR && tempIntersect.y <= yRange.y + ERROR &&
		tempIntersect.z >= zRange.x - ERROR && tempIntersect.z <= zRange.y + ERROR)
	{
		shortestT = tempT;
		closestIntersect = tempIntersect;
	}

	for (vector<glm::vec4>::iterator L = planes.begin() + 1; L != planes.end(); ++L)
	{
		tempT = -glm::dot((*L), p0) / glm::dot((*L), rayVector);
		tempIntersect = p0 + (tempT * rayVector);

		cout << "Temp Intersect: " << glm::to_string(tempIntersect) << endl;

		if (tempT >= 0 &&
			tempIntersect.x >= xRange.x - ERROR && tempIntersect.x <= xRange.y + ERROR &&
			tempIntersect.y >= yRange.x - ERROR && tempIntersect.y <= yRange.y + ERROR &&
			tempIntersect.z >= zRange.x - ERROR && tempIntersect.z <= zRange.y + ERROR)
		{
			if (tempT < shortestT || shortestT < 0)
			{
				shortestT = tempT;
				closestIntersect = tempIntersect;
			}
		}
	}

	return glm::vec4(glm::vec3(closestIntersect), shortestT);
}


