// IPickable.h
// Purpose: This is a interface that outlines the behavior that a pickable object/model
// should have.
//
// @author Michael Lavoie

#pragma once

#include <GLM/glm.hpp>
#include <vector>

class IPickable
{
public:
	virtual ~IPickable() {}

	virtual void Picked() = 0;
	virtual std::vector<glm::vec3> GetWorldVertices() = 0;
};

