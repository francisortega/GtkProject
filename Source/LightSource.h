// LightSource.h
// Purpose: An object representing a specific light source in the scene
//
// @author Philippe Thibault
#pragma once

#include "Model.h"
#include <glm/glm.hpp>

#include <GL/glew.h>

class LightSource {

public :
	LightSource();
	LightSource(glm::vec3 lightColor, float lightKc, float lightKq, float lightKl, glm::vec4 lightPosition);
	~LightSource();

private:
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float lightKc = 0.0f;
	float lightKl = 0.0f;
	float lightKq = 0.05f;
	glm::vec4 lightPosition = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f); // If w = 1.0f, we have a point light

};