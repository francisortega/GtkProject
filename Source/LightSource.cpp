// LightSource.cpp
// Purpose: An object representing a specific light source in the scene

// @author Philippe Thibault

#include "LightSource.h"

LightSource::LightSource() {

}

LightSource::LightSource(glm::vec3 lightColor, float lightKc, float lightKq, float lightKl, glm::vec4 lightPosition) {
	this->lightColor = lightColor;
	this->lightKc = lightKc;
	this->lightKq = lightKq;
	this->lightKl = lightKl;
	this->lightPosition = lightPosition;
}

LightSource::~LightSource() {

}