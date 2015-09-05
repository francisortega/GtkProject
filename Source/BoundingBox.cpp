// BoundingBox.cpp
// Purpose: This is a static class that provides utility functions for creating
//			bounding boxes.
//
// @author Michael Lavoie

#include "BoundingBox.h"

#include "glm/ext.hpp"
#include <iostream>


BoundingBox::~BoundingBox()
{
}


vector<glm::vec4> BoundingBox::CreateAABoundingBox(vector<glm::vec3> vertices, glm::vec2 &xRange, glm::vec2 &yRange, glm::vec2 &zRange, glm::mat4 transform)
{
	CalculateBoundaries(vertices, xRange, yRange, zRange);
	TransformAABoundingBox(transform, xRange, yRange, zRange);

	// These are simplified calculations of the box's plane equations
	glm::vec4 leftFacePlane = glm::vec4(-1, 0, 0, xRange.x);
	glm::vec4 rightFacePlane = glm::vec4(1, 0, 0, -xRange.y);

	glm::vec4 bottomFacePlane = glm::vec4(0, -1, 0, yRange.x);
	glm::vec4 topFacePlane = glm::vec4(0, 1, 0, -yRange.y);

	glm::vec4 backFacePlane = glm::vec4(0, 0, -1, zRange.x);
	glm::vec4 frontFacePlane = glm::vec4(0, 0, 1, -zRange.y);

	return { leftFacePlane, rightFacePlane, bottomFacePlane, topFacePlane, backFacePlane, frontFacePlane };
}

void BoundingBox::TransformAABoundingBox(glm::mat4 transform, glm::vec2 &xRange, glm::vec2 &yRange, glm::vec2 &zRange)
{
	vector<glm::vec3> boxVertices;

	// Create the box's vertices to be transformed
	boxVertices = { glm::vec3(xRange.x, yRange.x, zRange.x),
		glm::vec3(xRange.y, yRange.x, zRange.x),
		glm::vec3(xRange.x, yRange.x, zRange.y),
		glm::vec3(xRange.y, yRange.x, zRange.y),
		glm::vec3(xRange.x, yRange.y, zRange.x),
		glm::vec3(xRange.y, yRange.y, zRange.x),
		glm::vec3(xRange.x, yRange.y, zRange.y),
		glm::vec3(xRange.y, yRange.y, zRange.y) };

	for (std::vector<glm::vec3>::size_type i = 0; i != boxVertices.size(); i++)
	{
		boxVertices[i] = glm::vec3(transform * glm::vec4(boxVertices[i], 1));
	}

	CalculateBoundaries(boxVertices, xRange, yRange, zRange);
}

void BoundingBox::CalculateBoundaries(vector<glm::vec3> vertices, glm::vec2 &xRange, glm::vec2 &yRange, glm::vec2 &zRange)
{
	if (vertices.empty())
		return;

	// Set the min and max values to the first vertex
	xRange = glm::vec2(vertices[0].x);
	yRange = glm::vec2(vertices[0].y);
	zRange = glm::vec2(vertices[0].z);

	// Find the boundary values for the vertices
	for (vector<glm::vec3>::iterator it = vertices.begin() + 1; it != vertices.end(); ++it)
	{
		xRange.x = glm::min(it->x, xRange.x);
		xRange.y = glm::max(it->x, xRange.y);

		yRange.x = glm::min(it->y, yRange.x);
		yRange.y = glm::max(it->y, yRange.y);

		zRange.x = glm::min(it->z, zRange.x);
		zRange.y = glm::max(it->z, zRange.y);
	}
}