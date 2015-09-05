// BoundingBox.h
// Purpose: This is a static class that provides utility functions for creating
//			bounding boxes.
//
// @author Michael Lavoie

#pragma once

#include <vector>
#include <GLM/glm.hpp>

using namespace std;

class BoundingBox
{
public:
	~BoundingBox();

	// Calculates the axis-aligned bounding-box's axis ranges(range[0] = min, range[1] = max and returns the plane normals
	static vector<glm::vec4> CreateAABoundingBox(vector<glm::vec3> vertices, glm::vec2 &xRange, glm::vec2 &yRange, glm::vec2 &zRange, glm::mat4 transform);
	static void TransformAABoundingBox(glm::mat4 transform, glm::vec2 &xRange, glm::vec2 &yRange, glm::vec2 &zRange);
	static void CalculateBoundaries(vector<glm::vec3> vertices, glm::vec2 &xRange, glm::vec2 &yRange, glm::vec2 &zRange);

private:
	BoundingBox() {};
};

