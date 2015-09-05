// Picker.h
// Purpose: This class  keeps a collection of pickable objects and decides
// when an object has been picked given certain screen coordinates.
//
// @author Michael Lavoie

#pragma once

#include <vector>

#include "IPickable.h"
#include "Camera.h"

using namespace std;

// Singleton class
class Picker
{
public:
	~Picker();

	static Picker& getInstance()
	{
		static Picker instance;
		return instance;
	}

	void registerPickable(IPickable* pickable);

	// Given the x and y screen coordinates, will call the picked function of the picked world model object
	void pick(double xPos, double yPos, Camera* camera);

private:
	Picker() {};
	Picker(Picker const&);
	void operator=(Picker const&);

	// Returns an intersection point with the w set to the distance between the point and the point of intersection; w < 0 if no intersection was found
	glm::vec4 intersectionPoint(IPickable* pickable, glm::vec4 rayVector, glm::vec4 p0);

	vector<IPickable*> pickables;
};

